/*
 * ################################################################
 *
 * ProActive: The ProActive Linux Agent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 *
 * ################################################################
 */

/*
 * NodeStarter.cpp
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#include "NodeStarter.h"
#include <iostream>

NodeStarter::NodeStarter(string name, string java_class,
		string security_policy, string log4j_file, string proactive_home,
		string classpath, string java_bin) {
	Initialize(name, java_class, security_policy, log4j_file, proactive_home,
			classpath, java_bin);
}
NodeStarter::NodeStarter(const NodeStarter &node) {
	pid = node.pid;
	Initialize(node.name, node.java_class, node.security_policy,
			node.log4j_file, node.proactive_home, node.classpath, node.java_bin);
}
NodeStarter::~NodeStarter() {
}

void NodeStarter::run() {
	/* prevent zombie process when the JVM or shell is killed
	 * need so the JVM/bash can be checked through kill */
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
		LOG4CXX_ERROR(logger,
				"Unable to ignore SIGCHILD, JVM maybe left as zombies when"
				"stopped and the Watchers may malfunction"
				"depending on the implementation of kill");
	pid_t sid;
	LOG4CXX_DEBUG(logger, "Starting the node [" << name
			<< "] in a child process. " <<
			"Name ["<<name << "] " <<
			"Starter class [" << java_class<< "] "<<
			"Security policy [" << security_policy<< "] "<<
			"Log4j files [" << log4j_file<< "] "<<
			"ProActive home [" << proactive_home<< "] "<<
			"Classpath [" << classpath<< "] "<<
			"Java binary [" << java_bin<< "] ");

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
		RunCommand();
		//if we reach this command has failed
		LOG4CXX_ERROR(logger, "Error starting JVM, execl has failed");
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
void NodeStarter::Initialize(string name, string java_class,
		string security_policy, string log4j_file, string proactive_home,
		string classpath, string java_bin) {
	logger = log4cxx::Logger::getLogger("NodeStarter " + name);
	this->name = name;
	this->java_class = java_class;
	this->security_policy = security_policy;
	this->log4j_file = log4j_file;
	this->proactive_home = proactive_home;
	this->classpath = classpath;
	this->java_bin = java_bin;
}

int NodeStarter::RunCommand() {
	return execl(java_bin.c_str(), " ", DEFAULT_DSECURITY_MANAGER.c_str(),
			security_policy.c_str(), log4j_file.c_str(),
			proactive_home.c_str(), "-classpath", classpath.c_str(),
			java_class.c_str(), name.c_str(), (char *) 0);
}
