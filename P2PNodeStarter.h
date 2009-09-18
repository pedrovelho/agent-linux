/*
 * P2PNodeStarter.h
 *
 *  Created on: Sep 18, 2009
 *      Author: vasile
 */

#ifndef P2PNODESTARTER_H_
#define P2PNODESTARTER_H_

#include "NodeStarter.h"

class P2PNodeStarter: public NodeStarter {
public:
	P2PNodeStarter(string name, string java_class,
			string security_policy, string log4j_file, string proactive_home,
			string classpath, string java_bin, string contact);
	virtual ~P2PNodeStarter();
	int RunCommand();
private:
	string contact;
};

#endif /* P2PNODESTARTER_H_ */
