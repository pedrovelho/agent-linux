/*
 * CustomNodeStarter.cpp
 *
 *  Created on: Sep 18, 2009
 *      Author: vasile
 */

#include "CustomNodeStarter.h"

CustomNodeStarter::CustomNodeStarter(string name, string java_class,
		string security_policy, string log4j_file, string proactive_home,
		string classpath, string java_bin, string arguments) :
	NodeStarter(name, java_class, security_policy, log4j_file, proactive_home,
			classpath, java_bin) {
	this->arguments = arguments;
}

CustomNodeStarter::~CustomNodeStarter() {
	// TODO Auto-generated destructor stub
}
//FIXME parse the arguments and add to exec
int CustomNodeStarter::RunCommand() {
	return execl(java_bin.c_str(), " ", DEFAULT_DSECURITY_MANAGER.c_str(),
			security_policy.c_str(), log4j_file.c_str(),
			proactive_home.c_str(), "-classpath", classpath.c_str(),
			java_class.c_str(), name.c_str(), (char *) 0);
}
