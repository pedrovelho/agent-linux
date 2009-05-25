/*
 * NodeStarter.h
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#ifndef NODESTARTER_H_
#define NODESTARTER_H_

#include "Thread.h"

class NodeStarter: public Thread {
public:
	NodeStarter();
	virtual ~NodeStarter();
	void Execute(void *arg);
};

#endif /* NODESTARTER_H_ */
