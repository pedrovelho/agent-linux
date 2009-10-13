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
Watcher *Watcher::AdvertWatcher(int jvm_pid, int tick, AdvertAction *action,
		Event *event, DBus::ControllerProxy::pointer controller) {
	return new Watcher(jvm_pid, tick, action, event, controller, ADVERT);
}
//RMAction
Watcher *Watcher::RMWatcher(int jvm_pid, int tick, RMAction *action,
		Event *event, DBus::ControllerProxy::pointer controller) {
	Watcher *w = new Watcher(jvm_pid, tick, action, event, controller, RM);
	return w;

}
//P2PAction
Watcher *Watcher::P2PWatcher(int jvm_pid, int tick, P2PAction *action,
		Event *event, DBus::ControllerProxy::pointer controller) {
	Watcher *w = new Watcher(jvm_pid, tick, action, event, controller, P2P);
	return w;
}
//CustomAction
Watcher *Watcher::CustomWatcher(int jvm_pid, int tick, CustomAction *action,
		Event *event, DBus::ControllerProxy::pointer controller) {
	Watcher *w = new Watcher(jvm_pid, tick, action, event, controller, CUSTOM);
	return w;
}

Watcher::Watcher(int jvm_pid, int tick, Action *action, Event *event,
		DBus::ControllerProxy::pointer controller, ActionType action_type) {
	//initialize logger
	logger = log4cxx::Logger::getLogger("Watcher");
	pid = jvm_pid;
	this->tick = tick;
	stop = false;
	this->controller = controller;
	this->action_select = action_type;
	this->action = action;
	this->event = event;
}

Watcher::~Watcher() {
	// TODO Auto-generated destructor stub
}

void Watcher::run() {
	limit(event->GetMaxCPUUsage());
	renice(event->GetProcessPriority());
	LOG4CXX_DEBUG(logger, "Watcher thread started for node " <<
			action->GetNodeName() << " with JVM PID " << pid);
	while (!stop) {
		//		usleep takes microseconds, we use milliseconds
		LOG4CXX_TRACE(logger, "Checking " << pid << " with an interval of "
				<< tick << " milliseconds");
		usleep(tick * 1000);

		// if a signal cannot be sent
		// notify the Controller which will restart the JVM
		if (kill(pid, 0) == -1) {
			LOG4CXX_INFO(logger, "Node [" << action->GetNodeName()
					<< "] with pid [ " << pid << " has stopped ");
			LOG4CXX_DEBUG(logger, "Sleeping for the amount in restart delay");
			//restart_delay is in milliseconds
			usleep(action->GetRestartDelay() * 1000);
			LOG4CXX_DEBUG(logger, "Signaling the controller...");

			//			method call on Controller asking to start a JVM
			//			returns the pid to check
			pid = RestartNode(action_select);
			LOG4CXX_DEBUG(logger, "Restarted the node with PID " << pid);
			limit(event->GetMaxCPUUsage());
			renice(event->GetProcessPriority());
		} else {
			LOG4CXX_TRACE(logger, "Node " << action->GetNodeName() << " is alive");
		}
	}
	LOG4CXX_TRACE(logger, "Watcher loop has finished.");
}
long Watcher::GetRestartDelay() {
	return action->GetRestartDelay();
}

int Watcher::RestartNode(ActionType action_type) {
	string node_name = action->GetNodeName();
	string java_class = action->GetStarterClass();
	switch (action_type) {
	case ADVERT: {
		LOG4CXX_TRACE(logger, "Restarting node for Advert action... ");
		return controller->StartNode(node_name, java_class);
	}
	case RM: {
		LOG4CXX_TRACE(logger, "Restarting node for RM action... ");

		return controller->StartRMNode(node_name, java_class,
				dynamic_cast<RMAction*> (action)->GetUsername(),
				dynamic_cast<RMAction*> (action)->GetPassword(),
				dynamic_cast<RMAction*> (action)->GetURL());
	}
	case P2P: {
		LOG4CXX_TRACE(logger, "Restarting node for P2P action... ");
		return controller->StartP2PNode(node_name, java_class,
				dynamic_cast<P2PAction*> (action)->GetContact());
	}
	case CUSTOM: {
		LOG4CXX_TRACE(logger, "Restarting node for Custom action... ");
		return controller->StartCustomNode(node_name, java_class,
				dynamic_cast<CustomAction*> (action)->GetArguments());
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
	//	cpu_max = limit;
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
	fork_pid = fork();
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
		execlp("cpulimit", " ", "-p", process_pid.c_str(), "-l", max.c_str(),
				"-z", (char*) 0);
		//if we reach this, exec  has failed
		LOG4CXX_ERROR(logger, "Error running cpulimit, execl has failed");
		std::exit(1);
	}
	//failed to fork
	else if (fork_pid < 0) {
		LOG4CXX_ERROR(logger, "Failed to fork ");
	} else {
		LOG4CXX_DEBUG(logger, "Returning from parent");
		return;
	}
}
void Watcher::renice(string priority){
	string nice_value = "0";
	//linux priorities are between -20(high) and 19(low)
	if(priority == REALTIME) nice_value = "-20";
	if(priority == HIGH) nice_value = "-14";
	if(priority == ABOVE_NORMAL) nice_value = "-7";
	if(priority == NORMAL) nice_value = "0";
	if(priority == BELOW_NORMAL) nice_value = "10";
	if(priority == LOW) nice_value = "19";


	/* prevent zombie process */
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
		LOG4CXX_ERROR(logger,
				"Unable to ignore SIGCHILD");
	pid_t sid, fork_pid;
	//Fork parent process
	fork_pid = fork();
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

		LOG4CXX_TRACE(logger, "Reniceing JVM to " << nice_value << " priority");
		execlp("renice", " ","-n",nice_value.c_str(), "-p", process_pid.c_str(), (char*) 0);
		//if we reach this, exec  has failed
		LOG4CXX_ERROR(logger, "Error running renice, execl has failed");
		std::exit(1);
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
	return action->GetNodeName();
}
} //namespace paagent
