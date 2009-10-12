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
 * Watcher.cpp
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#include "Watcher.h"
namespace paagent {
//named constructors for different types of action restarts
//AdvertAction
Watcher *Watcher::AdvertWatcher(int jvm_pid, int tick, int restart_delay,
		string name, string java_class,
		DBus::ControllerProxy::pointer controller) {
	return new Watcher(jvm_pid, tick, restart_delay, name, java_class,
			controller, ADVERT);
}
//RMAction
Watcher *Watcher::RMWatcher(int jvm_pid, int tick, int restart_delay,
		string name, string java_class, string user, string password,
		string url, DBus::ControllerProxy::pointer controller) {
	Watcher *w = new Watcher(jvm_pid, tick, restart_delay, name, java_class,
			controller, RM);
	w->SetRMValues(user, password, url);
	return w;

}
//P2PAction
Watcher *Watcher::P2PWatcher(int jvm_pid, int tick, int restart_delay,
		string name, string java_class, string contact,
		DBus::ControllerProxy::pointer controller) {
	Watcher *w = new Watcher(jvm_pid, tick, restart_delay, name, java_class,
			controller, P2P);
	w->SetP2PValues(contact);
	return w;
}
//CustomAction
Watcher *Watcher::CustomWatcher(int jvm_pid, int tick, int restart_delay,
		string name, string java_class, string arguments,
		DBus::ControllerProxy::pointer controller) {
	Watcher *w = new Watcher(jvm_pid, tick, restart_delay, name, java_class,
			controller, CUSTOM);
	w->SetCustomValues(arguments);
	return w;
}

Watcher::Watcher(int jvm_pid, int tick, int restart_delay, string name,
		string java_class, DBus::ControllerProxy::pointer controller,
		ActionType action) {
	//initialize logger
	logger = log4cxx::Logger::getLogger("Watcher " + name);
	pid = jvm_pid;
	node_name = name;
	this->tick = tick;
	stop = false;
	this->restart_delay = restart_delay;
	this->controller = controller;
	this->action_select = action;
	this->java_class = java_class;
	cpu_max = 100;
}

Watcher::~Watcher() {
	// TODO Auto-generated destructor stub
}

void Watcher::run() {
	LOG4CXX_DEBUG(logger, "Watcher thread started for node " << node_name
			<< " with JVM PID " << pid);
	while (!stop) {
		//		usleep takes microseconds, we use milliseconds
		LOG4CXX_TRACE(logger, "Checking " << pid << " with an interval of "
				<< tick << " milliseconds");
		usleep(tick * 1000);

		// if a signal cannot be sent
		// notify the Controller which will restart the JVM
		if (kill(pid, 0) == -1) {
			LOG4CXX_INFO(logger, "Node [" << node_name << "] with pid [ " << pid
					<< " has stopped ");
			LOG4CXX_DEBUG(logger, "Sleeping for the amount in restart delay");
			//restart_delay is in milliseconds
			usleep(restart_delay * 1000);
			LOG4CXX_DEBUG(logger, "Signaling the controller...");

			//			method call on Controller asking to start a JVM
			//			returns the pid to check
			pid = RestartNode(action_select);
			LOG4CXX_DEBUG(logger, "Restarted the node with PID " << pid);
			limit(cpu_max);
		} else {
			LOG4CXX_TRACE(logger, "Node " << node_name << " is alive");
		}
	}
	LOG4CXX_TRACE(logger, "Watcher loop has finished.");
}
long Watcher::GetRestartDelay() {
	return restart_delay;
}
void Watcher::SetRMValues(string user, string password, string url) {
	this->user = user;
	this->password = password;
	this->url = url;
}

void Watcher::SetP2PValues(string contact) {
	this->contact = contact;
}
void Watcher::SetCustomValues(string arguments) {
	this->arguments = arguments;
}
int Watcher::RestartNode(ActionType action) {
	switch (action) {
	case ADVERT: {
		LOG4CXX_TRACE(logger, "Restarting node for Advert action... ");
		return controller->StartNode(node_name, java_class);
	}
	case RM: {
		LOG4CXX_TRACE(logger, "Restarting node for RM action... ");
		return controller->StartRMNode(node_name, java_class, user, password,
				url);
	}
	case P2P: {
		LOG4CXX_TRACE(logger, "Restarting node for P2P action... ");
		return controller->StartP2PNode(node_name, java_class, contact);
	}
	case CUSTOM: {
		LOG4CXX_TRACE(logger, "Restarting node for Custom action... ");
		return controller->StartCustomNode(node_name, java_class, arguments);
	}
	default: {
		LOG4CXX_WARN(logger, "Action type not defined, node will not be restarted");
		stop = true;
		return -1;
	}
	}
}

int Watcher::GetPid() {
	return pid;
}
void Watcher::StopWatcher() {
	stop = true;
}

void Watcher::limit(int limit) {
	//intialize cpu_max if limit is called from elsewhere
	//needed if the node is restarted
	cpu_max = limit;
	//	Usage: cpulimit TARGET [OPTIONS...]
	//	   TARGET must be exactly one of these:
	//	      -p, --pid=N        pid of the process
	//	      -e, --exe=FILE     name of the executable program file
	//	      -P, --path=PATH    absolute path name of the executable program file
	//	   OPTIONS
	//	      -l, --limit=N      percentage of cpu allowed from 0 to 100 (mandatory)
	//	      -v, --verbose      show control statistics
	//	      -z, --lazy         exit if there is no suitable target process, or if it dies
	//	      -h, --help         display this help and exit

	/* prevent zombie process */
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
		LOG4CXX_ERROR(logger,
				"Unable to ignore SIGCHILD");
	pid_t sid, fork_pid;
	//Fork parent process
	LOG4CXX_DEBUG(logger, "Befor fork ------------------------------");
	fork_pid = fork();
	LOG4CXX_DEBUG(logger, "After fork ------------------------------");

	//If the fork was successful execute child code
	if (fork_pid == 0) {
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
		// Create a new SID for the child process,
		sid = setsid();
		if (sid < 0) {
			LOG4CXX_ERROR(logger, "setsid() call failed");
		}
		/* Change the current working directory to prevent the current
		 directory from being locked and not being able to remove it. */
		chdir("/");

		std::stringstream out;
		out << pid;
		string process_pid = out.str();

		std::stringstream out1;

		out1 << limit;
		string max = out1.str();

		LOG4CXX_TRACE(logger, "Running command for " << process_pid << " with limit " << max);
		execlp("cpulimit", " ", "-p", process_pid.c_str(), "-l",
				max.c_str(), "-z", (char*) 0);
		//if we reach this, exec  has failed
		LOG4CXX_ERROR(logger, "Error running cpulimit, execl has failed");
		//		exit(1);
	}
	//failed to fork
	else if (fork_pid < 0) {
		LOG4CXX_ERROR(logger, "Failed to fork ");
	} else {
		LOG4CXX_DEBUG(logger, "Returning from parent");
		return;
	}
}

string Watcher::GetName() const {
	return node_name;
}
} //namespace paagent
