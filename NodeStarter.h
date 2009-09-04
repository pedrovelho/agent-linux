/*
 * NodeStarter.h
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#ifndef NODESTARTER_H_
#define NODESTARTER_H_
//run  ccgnu2-config --help  for libraries and includes
#include <string>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include "Constants.h"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

class NodeStarter {
private:
	log4cxx::LoggerPtr logger;
	string node_exec;
	string name;
	string shell;
	int pid;
public:
	NodeStarter(string shell, string node_executable,
			string node_name);
	NodeStarter(const NodeStarter &node);
	virtual ~NodeStarter();
	void start();
	int getPid();
};

#endif /* NODESTARTER_H_ */

