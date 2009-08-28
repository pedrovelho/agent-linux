////============================================================================
//// Name        : bleah.cpp
//// Author      : Vasile Jureschi
//// Version     :
//// Copyright   : GPL-2
//// Description : Hello World in C++, Ansi-style
////============================================================================
//
//#include <iostream>
//#include "Controller.h"
//#include "ConfigParser.h"
//#include "Watcher.h"
//
////#include <boost/function.hpp>
////#include <boost/thread.hpp>
////#include <boost/bind.hpp>
//
//#include <stdio.h>
//#include <unistd.h>
//#include <pthread.h>
//using namespace std;
//
//typedef void *(*threadfunc)(void *);
//
//int main() {
////	StatusChecker checker;
//	Controller *controller = new Controller();
//	string shell = "/bin/bash";
//	string node_exec = "/home/vasile/workspacePA/PA_new/trunk/bin/startNode.sh";
//	string node_name = "node1";
//	int pid_bleah = 15;
//	pid_bleah = controller->StartNode(shell, node_exec, node_name);
//	cout << pid_bleah;
////	controller->AssociateNodesPids(pid_bleah, node_name);
////	cout << endl << "start jvm return " << pid_bleah <<	endl;
////	controller->StartWatcher(shell, node_exec, node_name, pid_bleah);
//	Watcher * watcher;
////	boost::thread*b pThread = new boost::thread(
////	    boost::bind(
////	    &Watcher::Run,             // member function
////	    &watcher ) );              // instance of class
//
//	pthread_t w;
////	threadfunc t = Watcher::Run;
////	pthread_create(&w, NULL, &t, NULL);
//	//	//sleep(5);
////
//////
//////	controller->StartJVM(shell, node_exec, "node2");
//////	controller->StartJVM(shell, node_exec, "node3");
////
//////	cout << "Trying to set tick"<< endl;
//////	controller->SetTick(pid_bleah, 10);
//////	sleep(5);
////	cout << "gah " <<controller->GetTick(pid_bleah) << endl;
////	controller->SetTick(pid_bleah, 4000);
////	cout << controller->GetTick(pid_bleah) << endl;
////
////	sleep(10);
//////	cout << "Trying to change watcher to pid mappings" << endl;
//////	controller->SetWatcherPid(56,23);
////	cout << controller->GetWatcherPid(pid_bleah);
//////
//////	controller->SetWatcherPid(56,123);
//////	cout << controller->GetWatcherPid(56);
////
////
////
////	cout << "Stopping watcher " <<endl;
////	cout << "oprit ?  "<< controller->StopWatcher(node_name);
//////	controller->StopWatcher(pid_bleah);
////	sleep(10);
////	wait();
////	std::string file = "/home/vasile/workspacePA/agent_svn/SHORT_agent_manual/PAAgent-version.xml";
////	std::string file = "/home/vasile/workspacePA/linux-agent/ConfigExample.xml";
////	ConfigParser *parse = new ConfigParser(file);
////	parse->startNodes();
//
//}

#include <iostream>

#include <string>
#include "Controller.h"
#include "Watcher.h"
#include "DBusMessaging.h"
using namespace std;
using namespace ost;
int main() {

		Controller *controller = new Controller();
		string shell = "/bin/bash";
		string node_exec = "/home/vasile/workspacePA/PA_new/trunk/bin/startNode.sh";
		string node_name = "node1";
		controller->SetDefaultNodeStarter(node_exec);
		controller->SetDefaultShell(shell);
//
		DBusMessaging *messenger = DBusMessaging::Inst("pa.controller");
		int pid_bleah = 15;
		pid_bleah = controller->StartNode(shell, node_exec, node_name);
		int pid_bleah1 = controller->StartNode(shell, node_exec, node_name + node_name);

		Watcher watcher(node_name,pid_bleah,3000);
		watcher.start();
//		Watcher watcher1("fake",3000,3000);
//		watcher1.start();
		Thread::sleep(100000000000);
		cout << "sdsfsdffffffffffffffffffffffffffffff" <<  endl;
		return 0;

}
