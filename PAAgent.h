/*
 * PAAgent.h
 *
 *  Created on: Sep 1, 2009
 *      Author: vasile
 */

#ifndef PAAGENT_H_
#define PAAGENT_H_

#include <cc++/thread.h>
using namespace ost;
class PAAgent: public Thread {
public:
	PAAgent();
	virtual ~PAAgent();
	void run();

};
#endif /* PAAGENT_H_ */
