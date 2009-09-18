/*
 * Watcher.cpp
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#include "Watcher.h"

//named constructors for different types of action restarts
//AdvertAction
Watcher Watcher::AdvertWatcher(int jvm_pid, int tick, int restart_delay,
		string name, string java_class, DBus::ControllerProxy::pointer controller) {
	Watcher w(jvm_pid, tick, restart_delay, name, java_class, controller, ADVERT);
	return w;
}
//RMAction
Watcher Watcher::RMWatcher(int jvm_pid, int tick, int restart_delay,
		string name, string java_class, string user, string password,
		string url, DBus::ControllerProxy::pointer controller) {
	Watcher w(jvm_pid, tick, restart_delay, name, java_class, controller, RM);
	w.SetRMValues(user, password, url);
	return w;

}
//P2PAction
Watcher Watcher::P2PWatcher(int jvm_pid, int tick, int restart_delay,
		string name, string java_class, string contact,
		DBus::ControllerProxy::pointer controller) {
	Watcher w(jvm_pid, tick, restart_delay, name, java_class, controller, P2P);
	w.SetP2PValues(contact);
	return w;
}
//CustomAction
Watcher Watcher::CustomWatcher(int jvm_pid, int tick, int restart_delay,
		string name, string java_class, string arguments,
		DBus::ControllerProxy::pointer controller) {
	Watcher w(jvm_pid, tick, restart_delay, name, java_class, controller, CUSTOM);
	w.SetCustomValues(arguments);
	return w;
}

Watcher::Watcher(int jvm_pid, int tick, int restart_delay, string name, string java_class,
		DBus::ControllerProxy::pointer controller, ActionType action) {
	//initialize logger
	//TODO  move outside the constructor
	logger = log4cxx::Logger::getLogger("Watcher " + name);
	logger->setLevel(log4cxx::Level::getTrace());
	pid = jvm_pid;
	node = name;
	this->tick = tick;
	stop = false;
	this->restart_delay = restart_delay;
	this->controller = controller;
	this->action_select = action;
	this->java_class = java_class;
}

Watcher::~Watcher() {
	// TODO Auto-generated destructor stub
}

void Watcher::run() {
	LOG4CXX_DEBUG(logger, "Watcher thread started for node " << node
			<< " with JVM PID " << pid);
	while (!stop) {
		//		usleep takes microseconds, we use milliseconds
		LOG4CXX_TRACE(logger, "Checking " << pid << " with an interval of "
				<< tick << " milliseconds");
		usleep(tick * 1000);

		// if a signal cannot be sent
		// notify the Controller which will restart the JVM
		if (kill(pid, 0) == -1) {
			LOG4CXX_INFO(logger, "Node [" << node << "] with shell [ " << pid
					<< " seems to have been stopped ");
			LOG4CXX_DEBUG(logger, "Sleeping for the amount in restart delay");
			//restart_delay is in milliseconds
			usleep(restart_delay * 1000);
			LOG4CXX_DEBUG(logger, "Signaling the controller...");

			//			method call on Controller asking to start a JVM
			//			returns the pid to check
			pid = RestartNode(action_select);
			//stop = true;

		} else {
			LOG4CXX_TRACE(logger, "Node " << node << " is alive");
		}
	}
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
		return controller->StartNode(node,java_class);
		break;
	}
	case RM: {
		return controller->StartRMNode(node, java_class, user, password, url);
		break;
	}
	case P2P: {
		return controller->StartP2PNode(node,java_class, contact);
		break;
	}
	case CUSTOM: {
		return controller->StartCustomNode(node, java_class, arguments);
		break;
	}
	default: {
		LOG4CXX_WARN(logger, "Action type not defined, node will not be restarted");
		stop  = true;
		return -1;
	}
	}
}
