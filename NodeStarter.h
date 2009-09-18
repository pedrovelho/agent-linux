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

//#include <cc++/thread.h>
//using namespace ost;
class NodeStarter{
public:
	//	int piexec = execl(DEFAULT_JAVA_BIN.c_str(), " ",
	//			DEFAULT_DSECURITY_MANAGER.c_str(),
	//			DEFAULT_DJAVA_SECURITY.c_str(),
	//			DEFAULT_DLOG4J_FILE.c_str(),
	//			DEFAULT_DPROACTIVE_HOME.c_str(),
	//			"-classpath", DEFAULT_DCLASSPATH.c_str(),
	//			DEFAULT_JAVA_CLASS.c_str(), "node1", (char *) 0);

	NodeStarter(string name, string java_class = DEFAULT_JAVA_CLASS,
			string security_policy = DEFAULT_DJAVA_SECURITY, string log4j_file =
					DEFAULT_DLOG4J_FILE, string proactive_home =
					DEFAULT_DPROACTIVE_HOME, string classpath = DEFAULT_DCLASSPATH,
			string java_bin = DEFAULT_JAVA_BIN);
	NodeStarter(const NodeStarter &node);
	virtual ~NodeStarter();
	virtual void run();
	int getPid();
	int RunCommand();
protected:
	string name;
	string java_class;
	string security_policy;
	string log4j_file;
	string proactive_home;
	string classpath;
	string java_bin;
private:
	log4cxx::LoggerPtr logger;
	int pid;

	void Initialize(string name, string java_class, string security_policy,
			string log4j_file, string proactive_home, string classpath,
			string java_bin);
};

#endif /* NODESTARTER_H_ */

