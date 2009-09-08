/*
 * NodeStarter.cpp
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#include "NodeStarter.h"

NodeStarter::NodeStarter(string _shell = DEFAULT_SHELL, string node_executable =
		DEFAULT_NODE_EXEC, string node_name = DEFAULT_NODE) {
	//initialize logger
	//TODO  move outside the constructor
	logger = log4cxx::Logger::getLogger("NodeStarter " + node_name);
	logger->setLevel(log4cxx::Level::getTrace());
	shell = _shell;
	node_exec = node_executable;
	name = node_name;
}
NodeStarter::NodeStarter(const NodeStarter &node) {
	logger = log4cxx::Logger::getLogger("NodeStarter " + node.name);
	logger->setLevel(log4cxx::Level::getTrace());
	shell = node.shell;
	node_exec = node.node_exec;
	name = node.name;
	pid = node.pid;
}
NodeStarter::~NodeStarter() {
}

void NodeStarter::start() {
	/* prevent zombie process when the JVM or shell is killed
	 * need so the JVM/bash can be checked through kill */
	if (signal(SIGCHLD, SIG_IGN))
		LOG4CXX_ERROR(logger,
				"Unable to ignore SIGCHILD, bash will be left as "
					"a zombie when JVM is killed and the Watchers may malfunction"
					"depending on the implementation of kill");
	pid_t sid;
	LOG4CXX_DEBUG(logger, "Starting the node [" << name
			<< "] in a child process "
				"using [" << node_exec << "] with shell [" << shell << "]");
	//Fork parent process
	pid = fork();
	//If the fork was successful execute child code
	if (pid == 0) {
		// Running in child process
		//Decouple from parent (controller) environment
		/*Change the file mode process to:
		 * umask value   User  Group Others
		 *
		 * 0000       all   all   all
		 * 0007       all   all   none
		 * 0027       all   r/w   none
		 */
		umask(27);
		// Create a new SID for the child process, this prevents the
		// nodes from stopping if the controller stops
		sid = setsid();
		if (sid < 0) {
			LOG4CXX_ERROR(logger, "setsid() call failed");
		}
		/* Change the current working directory to prevent the current
		 directory from being locked and not being able to remove it. */
		chdir("/");
		//Decoupling from parent environment done, running command
		int piexec = execl(shell.c_str(), " -c ", node_exec.c_str(),
				name.c_str(), name.c_str(), (char *) 0);

		//if the node fails to start in the child process return -1
		//otherwise return the pid (=0)

	}
	//failed to fork
	else if (pid < 0) {
		LOG4CXX_ERROR(logger, "Failed to fork when trying to start a node");
	} else {
		LOG4CXX_DEBUG(logger, "Returning from parent");
		return;
	}
	LOG4CXX_DEBUG(logger, "Node [" << name << "] started");

}
int NodeStarter::getPid() {

	return pid;
}
