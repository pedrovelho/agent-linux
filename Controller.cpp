/*
 * Controller.cpp
 *
 *  Created on: Mar 19, 2009
 *      Author: vasile
 */

#include "NodeStarter.h"
#include "Controller.h"

Controller::Controller() {
	//initialize logger
	//TODO  move outside the constructor
//	logger = log4cxx::Logger::getLogger("Controller");
//	BasicConfigurator::configure();
//	logger->setLevel(log4cxx::Level::getTrace());
}
//Controller::Controller(const Controller &controller){
//	default_node_exec = controller.default_node_exec;
//	default_shell = controller.default_shell;
////	logger =  log4cxx::Logger::getLogger("Controller");
////	logger->setLevel(log4cxx::Level::getTrace());
//}
Controller::~Controller() {
}
int Controller::StartNode(string name, string java_class,
		string security_policy, string log4j_file, string proactive_home,
		string classpath, string java_bin) {

	NodeStarter starter(name, java_class, security_policy, log4j_file,
			proactive_home, classpath, java_bin);
	starter.start();

	//	LOG4CXX_DEBUG(logger, "Node started");
	return starter.getPid();
}

int Controller::StartNode(string node_name) {
	NodeStarter starter(node_name);
	starter.start();
	return starter.getPid();
}
bool Controller::StopNode(int pid) {
	//try to stop

	kill(pid, SIGTERM);
	//wait for the JVM to stop
	sleep(SLEEP_TIME_AFTER_KILL);
	bool dead = false;
	//return check for stop,
	//kill(pid, 0) returns -1 if the process does not exist
	int stop = kill(pid, 0);
	LOG4CXX_DEBUG(logger, "Kill command return after trying to stop the node " << stop);
	if (stop != 0) {
		dead = true;
	}
	return dead;
}

bool Controller::StopNode(string name){
	LOG4CXX_ERROR(logger,"Method not implemented, use stop by PID");
	return false;
}
LoggerPtr Controller::getLogger() {
	return logger;
}

