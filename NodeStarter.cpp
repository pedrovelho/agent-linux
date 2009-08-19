/*
 * NodeStarter.cpp
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#include "NodeStarter.h"

NodeStarter::NodeStarter(string shell, string node_executable,
		string node_name) {
	//initialize logger
	//TODO  move outside the constructor
	logger = log4cxx::Logger::getLogger("NodeStarter " + node_name);
	BasicConfigurator::configure();
	logger->setLevel(log4cxx::Level::getTrace());
	this->shell = shell;
	this->node_exec = node_exec;
	this->name = node_name;
}

NodeStarter::~NodeStarter() {
	// TODO Auto-generated destructor stub
}

void NodeStarter::run(){
		pid_t pid, sid;

		/* already a daemon */
		//	if ( getppid() == 1 ) return 0;

		/* Fork off the parent process */
		pid = fork();
		//If the fork was successful execute child code
		if (pid == 0) {
			/* At this point we are executing as the child process */
			/* Change the file mode mask */
			umask(0);
			/* Create a new SID for the child process */
	//		sid = setsid();
			LOG4CXX_DEBUG(logger, "Started the node in a child process");
			//get the shotgun ready for zombies ?
			//
			//		if (sid < 0) {
			//			exit(EXIT_FAILURE);
			//		}

			/* Change the current working directory.  This prevents the current
			 directory from being locked; hence not being able to remove it. */
			chdir("/");
			/* Redirect standard files to /dev/null */
			//		freopen( "/dev/null", "r", stdin);
			//		freopen( "/dev/null", "w", stdout);
			//		freopen( "/dev/null", "w", stderr);

	//		int piexec = execlp(shell.c_str(), "-c", node_executable.c_str(),
	//				node_name.c_str(), (char *) 0);

			int piexec = execlp(node_exec.c_str(),name.c_str(),
					name.c_str(), (char *) 0);

			//if the node fails to start in the child process return -1
			//otherwise return the pid (=0)

		}
		//failed to fork
		else if (pid < 0) {
			LOG4CXX_ERROR(logger, "Failed to fork when trying to start a node");
		} else {
			LOG4CXX_DEBUG(logger, "Returning from parent");
			//add to the list of pids and set a default tick
		}

}
int NodeStarter::getPid(){
	return pid;
}
