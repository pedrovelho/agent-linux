/*
 * P2PNodeStarter.cpp
 *
 *  Created on: Sep 18, 2009
 *      Author: vasile
 */

#include "P2PNodeStarter.h"

P2PNodeStarter::P2PNodeStarter(string name, string java_class,
		string security_policy, string log4j_file, string proactive_home,
		string classpath, string java_bin, string contact) :
	NodeStarter(name, java_class, security_policy, log4j_file, proactive_home,
			classpath, java_bin) {
	this->contact = contact;
}

P2PNodeStarter::~P2PNodeStarter() {
	// TODO Auto-generated destructor stub
}
//what's contact good for ?
int P2PNodeStarter::RunCommand() {
	return execl(java_bin.c_str(), " ", DEFAULT_DSECURITY_MANAGER.c_str(),
			security_policy.c_str(), log4j_file.c_str(),
			proactive_home.c_str(), "-classpath", classpath.c_str(),
			java_class.c_str(), name.c_str(), (char *) 0);
}

