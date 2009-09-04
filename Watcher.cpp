/*
 * Watcher.cpp
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#include "Watcher.h"


Watcher::Watcher(string node_name, int jvm_pid, int tick) {
	//initialize logger
	//TODO  move outside the constructor
	logger = log4cxx::Logger::getLogger("Watcher " + node_name);
	logger->setLevel(log4cxx::Level::getDebug());
	pid = jvm_pid;
	node = node_name;
	this->tick=tick;
	stop = false;
}

Watcher::~Watcher() {
	// TODO Auto-generated destructor stub
}

void Watcher::run() {
	LOG4CXX_DEBUG(logger, "Watcher thread started for node " <<
			node << " with JVM PID " << pid );
	while (!stop) {
		//		usleep takes microseconds, we use milliseconds
		LOG4CXX_TRACE(logger, "Checking "<< pid << " with an interval of " << tick << " milliseconds" );
		usleep(tick*1000);


		// if a signal cannot be sent
		// notify the Controller which will restart the JVM
		if (kill(pid, 0) == -1) {
			LOG4CXX_INFO(logger, "Node [" << node << "] with shell [ "
					<< pid << " seems to have been stopped " );
			LOG4CXX_DEBUG(logger, "Signaling the controller...");

			//method call on Controller asking to start a JVM
			//returns the pid to check
			//stop = true;
		}
		else{
			LOG4CXX_TRACE(logger, "Node " << node << " is alive");
		}
	}
}

