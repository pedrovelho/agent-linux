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
namespace paagent {
NodeStarter::NodeStarter(string name, string java_class) {
	logger = log4cxx::Logger::getLogger("NodeStarter");
	this->name = name;
	this->java_class =java_class;
	Initialize();
}
NodeStarter::~NodeStarter() {
}

void NodeStarter::run() {
	Configuration *config = Configuration::Inst();

	/* prevent zombie process when the JVM or shell is killed
	 * need so the JVM/bash can be checked through kill */
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
		LOG4CXX_ERROR(logger,
				"Unable to ignore SIGCHILD, JVM maybe left as zombies when"
				"stopped and the Watchers may malfunction"
				"depending on the implementation of kill");
	pid_t sid;
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
		//if we reach this, command has failed
		LOG4CXX_ERROR(logger, "Error starting JVM, execl has failed");
		exit(1);
	}
	//failed to fork
	else if (pid < 0) {
		LOG4CXX_ERROR(logger, "Failed to fork when trying to start a node");
	} else {
		LOG4CXX_DEBUG(logger, "Returning from parent");
		return;
	}
}
int NodeStarter::getPid() {
	return pid;
}
void NodeStarter::Initialize() {
	Configuration *config = Configuration::Inst();
	this->java_bin = config->GetJavaBin();
	string pa_location = config->GetPALocation();
	string java_security = DEFAULT_DJAVA_SECURITY_OPTION + pa_location
			+ DEFAULT_DJAVA_SECURITY_FILE;
	string log4j_configuration = DEFAULT_DLOG4J_OPTION + pa_location
			+ DEFAULT_DLOG4J_FILE;
	string pa_home_option = DEFAULT_DPROACTIVE_OPTION + pa_location;
	vector<string> arguments = config->GetJVMParams();
	string dash = "-";
	for (int i = 0; i < arguments.size(); i++) {
		exec_arguments.push_back(dash + arguments.at(i));
	}
	exec_arguments.push_back(DEFAULT_DSECURITY_MANAGER);
	exec_arguments.push_back(java_security);
	exec_arguments.push_back(log4j_configuration);
	exec_arguments.push_back(pa_home_option);
	exec_arguments.push_back("-classpath");
	exec_arguments.push_back(config->GetClasspath());
	exec_arguments.push_back(java_class);
	exec_arguments.push_back(name);
}

int NodeStarter::RunCommand() {
	char *argv[exec_arguments.size() + 1];
	for (int i = 0; i < exec_arguments.size(); i++) {
		argv[i] = (char *) exec_arguments.at(i).c_str();
	}
	argv[exec_arguments.size()] = (char *) 0;

	LOG4CXX_DEBUG(logger,"Starting node with the following parameters");
	for (int i = 0; i < (exec_arguments.size() + 1); i++) {
		LOG4CXX_DEBUG(logger, argv[i]);
	}
	LOG4CXX_DEBUG(logger, java_bin);

	return execv(java_bin.c_str(), argv);
}
} //namespace paagent
