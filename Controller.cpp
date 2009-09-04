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
////	BasicConfigurator::configure();
//	logger->setLevel(log4cxx::Level::getTrace());
	default_shell = DEFAULT_SHELL;
	default_node_exec = DEFAULT_NODE_EXEC;
}
//Controller::Controller(const Controller &controller){
//	default_node_exec = controller.default_node_exec;
//	default_shell = controller.default_shell;
////	logger =  log4cxx::Logger::getLogger("Controller");
////	logger->setLevel(log4cxx::Level::getTrace());
//}
Controller::~Controller() {
}

int Controller::StartNode(string shell, string node_executable,
		string node_name) {


	NodeStarter starter(shell, node_executable,node_name);
	starter.start();

//	LOG4CXX_DEBUG(logger, "Node started");
	return starter.getPid();

}

int Controller::StartNode(string node_name) {
	return StartNode(node_name);
}
//FIXME broken !!!, doesn't always stop jvms
bool Controller::StopNode(int pid){
	StopWatcher(pid);
	//try to stop

	kill(pid, 15);
	bool dead = false;
	//return check for stop,
	//kill(pid, 0) returns -1 if the process does not exist
	if (kill(pid, 0) == -1) dead = true;
	return  dead;
}
void Controller::SetTick(int pid, int tick) {
	LOG4CXX_TRACE(logger, "Changing tick to " << tick << " for pid " << pid );
//	pids_ticks.insert(pair<int, int> (pid, tick));
//	LOG4CXX_TRACE(logger, "The size of the maps containing the pids and ticks is now " << pids_ticks.size());
}

void Controller::SetWatcherPid(int process_pid, int watcher_pid) {

	LOG4CXX_ERROR(logger, "SetWatchet pid method not implemented ");

}

int Controller::GetWatcherPid(int process_pid) {

	LOG4CXX_ERROR(logger, "Method not implemented");
	return false;

}

int Controller::GetTick(int pid) {
//	map<int, int>::iterator iter;
////	iter = pids_ticks.find(pid);
//	int tick = 0;
//	if (iter != pids_ticks.end()) {
//		tick = iter->second;http://www.google.com/reader/view/#stream/user%2F15650323335570657892%2Flabel%2Fcool
//		LOG4CXX_TRACE(logger, "Tick value retrieved for pid " << pid << " is " << tick );
//	} else {
//		LOG4CXX_ERROR(logger, "No tick value found for pid " << pid);
//	}
	return 1000;
}

bool Controller::StopWatcher(int pid) {

	LOG4CXX_ERROR(logger, "Method not implemented ");
	return false;
}

bool Controller::StopWatcher(string node_name) {

	//	    }
	LOG4CXX_ERROR(logger, "Method not implemented ");
	return false;

}

void Controller::ResetTicks() {

}

void Controller::SetDefaultShell(string shell) {
	default_shell = shell;
}
string Controller::GetDefaultShell() {
	return default_shell;
}
void Controller::SetDefaultNodeStarter(string node_executable) {
	default_node_exec = node_executable;
}
string Controller::GetDefaultNodeStarter() {
	return default_node_exec;
}

LoggerPtr Controller::getLogger() {
	return logger;
}




