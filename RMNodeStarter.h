/*
 * RMNodeStarter.h
 *
 *  Created on: Sep 16, 2009
 *      Author: vasile
 */

#ifndef RMNODESTARTER_H_
#define RMNODESTARTER_H_

#include "NodeStarter.h"

class RMNodeStarter: public NodeStarter {
public:
	RMNodeStarter(string name, string java_class, string security_policy,
			string log4j_file, string proactive_home, string classpath,
			string java_bin, string user, string password, string url);
	virtual ~RMNodeStarter();
	int RunCommand();
private:
	string user;
	string password;
	string url;
};

#endif /* RMNODESTARTER_H_ */
