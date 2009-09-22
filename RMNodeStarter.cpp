/*
 * RMNodeStarter.cpp
 *
 *  Created on: Sep 16, 2009
 *      Author: vasile
 */

#include "RMNodeStarter.h"

RMNodeStarter::RMNodeStarter(string name, string java_class,
		string security_policy, string log4j_file, string proactive_home,
		string classpath, string java_bin, string user, string password,
		string url) :
	NodeStarter(name, java_class, security_policy, log4j_file, proactive_home,
			classpath, java_bin) {
	this->user = user;
	this->password = password;
	this->url = url;

}

RMNodeStarter::~RMNodeStarter() {
	// TODO Auto-generated destructor stub
}
//FIXME this doesn't use the url, password and user name
//find out the command syntax and use here !!!
int RMNodeStarter::RunCommand() {
	return execl(java_bin.c_str(), " ", DEFAULT_DSECURITY_MANAGER.c_str(),
			security_policy.c_str(), log4j_file.c_str(),
			proactive_home.c_str(), "-classpath", classpath.c_str(),
			java_class.c_str(), name.c_str(), (char *) 0);
}
