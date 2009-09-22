/*
 * Controller.cpp
 *
 *  Created on: Mar 19, 2009
 *      Author: vasile
 */

#include "RMNodeStarter.h"
#include "CustomNodeStarter.h"
#include "P2PNodeStarter.h"
#include "NodeStarter.h"
#include "Controller.h"
#include "Constants.h"

Controller::Controller() {
	logger = log4cxx::Logger::getLogger("Controller");
	//initialize to defaults for safety
	security_policy = DEFAULT_DJAVA_SECURITY;
	log4j_file = DEFAULT_DLOG4J_FILE;
	proactive_home = DEFAULT_PROACTIVE_HOME;
	classpath = DEFAULT_DCLASSPATH;
	java_bin = DEFAULT_JAVA_BIN;
}
//Controller::Controller(const Controller &controller){
//	default_node_exec = controller.default_node_exec;
//	default_shell = controller.default_shell;
////	logger =  log4cxx::Logger::getLogger("Controller");
//}
Controller::~Controller() {
}
int Controller::StartNode(string name, string java_class) {

	NodeStarter starter(name, java_class, security_policy, log4j_file,
			proactive_home, classpath, java_bin);
	starter.run();
	//	LOG4CXX_DEBUG(logger, "Node started");
	return starter.getPid();
}

int Controller::StartRMNode(string name, string java_class, string user,
		string password, string url) {

	RMNodeStarter starter(name, java_class, security_policy, log4j_file,
			proactive_home, classpath, java_bin, user, password, url);
	starter.run();
	//	LOG4CXX_DEBUG(logger, "Node started");
	return starter.getPid();
}
int Controller::StartP2PNode(string name, string java_class, string contact) {
	P2PNodeStarter starter(name, java_class, security_policy, log4j_file,
			proactive_home, classpath, java_bin, contact);
	starter.run();
	//	LOG4CXX_DEBUG(logger, "Node started");
	return starter.getPid();

}

int Controller::StartCustomNode(string name, string java_class,
		string arguments) {
	CustomNodeStarter starter(name, java_class, security_policy, log4j_file,
			proactive_home, classpath, java_bin, arguments);
	starter.run();
	//	LOG4CXX_DEBUG(logger, "Node started");
	return starter.getPid();
}

int Controller::StopNode(int pid) {
	//try to stop

	kill(pid, SIGTERM);
	//wait for the JVM to stop
	//	sleep(SLEEP_TIME_AFTER_KILL);
	bool dead = false;
	//return check for stop,
	//kill(pid, 0) returns -1 if the process does not exist
	int stop = kill(pid, 0);
	LOG4CXX_DEBUG(logger, "Kill command return after trying to stop the node " << stop);
	if (stop != 0) {
		dead = true;
	}
	//	return dead;
	return 0;
}

bool Controller::StopNode(string name) {
	LOG4CXX_ERROR(logger,"Method not implemented, use stop by PID");
	return false;
}
int Controller::SetStartConfiguration(string security_policy,
		string log4j_file, string proactive_home, string classpath,
		string java_bin) {
	this->security_policy = security_policy;
	this->log4j_file = log4j_file;
	this->proactive_home = proactive_home;
	this->classpath = classpath;
	this->java_bin = java_bin;
	return 0;
}

