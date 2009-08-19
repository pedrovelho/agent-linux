/*
 * NodeStarter.h
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#ifndef NODESTARTER_H_
#define NODESTARTER_H_
//run  ccgnu2-config --help  for libraries and includes
#include <cc++/thread.h>
#include <string>
#include <stdlib.h>
using namespace ost;

#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"
using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;


class NodeStarter: public Thread {
private:
	log4cxx::LoggerPtr logger;
	string node_exec;
	string name;
	string shell;
	int pid;
public:
	NodeStarter(string shell, string node_executable,
			string node_name);
	virtual ~NodeStarter();
	void run();
	int getPid();
};

#endif /* NODESTARTER_H_ */
