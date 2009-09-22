/*
 * CustomNodeStarter.h
 *
 *  Created on: Sep 18, 2009
 *      Author: vasile
 */

#ifndef CUSTOMNODESTARTER_H_
#define CUSTOMNODESTARTER_H_

#include "NodeStarter.h"

class CustomNodeStarter: public NodeStarter {
public:
	CustomNodeStarter(string name, string java_class, string security_policy,
			string log4j_file, string proactive_home, string classpath,
			string java_bin, string arguments);
	virtual ~CustomNodeStarter();
	int RunCommand();
private:
	string arguments;
};

#endif /* CUSTOMNODESTARTER_H_ */
