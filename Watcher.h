/*
 * Watcher.h
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#ifndef GENERICS
#define GENERICS

#endif

#ifndef WATCHER_H_
#define WATCHER_H_
#include <iostream>
#include <signal.h>
#include <string>

//run  ccgnu2-config --help  for libraries and includes
#include <cc++/thread.h>
using namespace ost;

#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"
using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

//#include "Controller.h"
#include "DBusMessaging.h"
#include "Constants.h"
class Watcher: public Thread {
private:
	log4cxx::LoggerPtr logger;
	int pid;
	string node;
	int tick;
	bool stop;
//	Controller *controller;
public:
	Watcher() {};
	Watcher(string node_name, int jvm_pid, int tick);
	int GetTick();
	int GetPid();
	//	void SetTick();
	void SetPid();
	void Start();
	virtual ~Watcher();
	void run();

};

#endif /* WATCHER_H_ */
