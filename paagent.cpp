//============================================================================
// Name        : bleah.cpp
// Author      : Vasile Jureschi
// Version     :
// Copyright   : GPL-2
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Controller.h"
#include "ConfigParser.h"
using namespace std;

int main() {
//	StatusChecker checker;
//	Controller *controller = new Controller();
//	char *shell = "/bin/bash";
//	char *node_exec = "/home/vasile/workspacePA/PA_new/trunk/bin/startNode.sh";
//	char *node_name = "node1";
//	int pid_bleah = 15;
//	pid_bleah = controller->StartNode(shell, node_exec, node_name);
//	controller->AssociateNodesPids(pid_bleah, node_name);
//	cout << endl << "start jvm return " << pid_bleah <<	endl;
//	controller->StartWatcher(shell, node_exec, node_name, pid_bleah);
//	//sleep(5);
//
////
////	controller->StartJVM(shell, node_exec, "node2");
////	controller->StartJVM(shell, node_exec, "node3");
//
////	cout << "Trying to set tick"<< endl;
////	controller->SetTick(pid_bleah, 10);
////	sleep(5);
//	cout << "gah " <<controller->GetTick(pid_bleah) << endl;
//	controller->SetTick(pid_bleah, 4000);
//	cout << controller->GetTick(pid_bleah) << endl;
//
//	sleep(10);
////	cout << "Trying to change watcher to pid mappings" << endl;
////	controller->SetWatcherPid(56,23);
//	cout << controller->GetWatcherPid(pid_bleah);
////
////	controller->SetWatcherPid(56,123);
////	cout << controller->GetWatcherPid(56);
//
//
//
//	cout << "Stopping watcher " <<endl;
//	cout << "oprit ?  "<< controller->StopWatcher(node_name);
////	controller->StopWatcher(pid_bleah);
//	sleep(10);
//	wait();
//	std::string file = "/home/vasile/workspacePA/agent_svn/SHORT_agent_manual/PAAgent-version.xml";
	std::string file = "/home/vasile/workspacePA/linux-agent/ConfigExample.xml";
	ConfigParser *parse = new ConfigParser(file);
	parse->startNodes();

}
