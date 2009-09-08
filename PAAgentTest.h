/*
 * PAAgentTest.h
 *
 *  Created on: Sep 3, 2009
 *      Author: vasile
 */

#ifndef PAAGENTTEST_H_
#define PAAGENTTEST_H_
#include <gtest/gtest.h>
#include "PAAgent.h"
#include "controller_proxy.h"

class PAAgentTest: public ::testing::Test {
public:
	PAAgentTest();
	virtual ~PAAgentTest();
	virtual void SetUp();
	//stops all JVMS
	virtual void TearDown();
};

#endif /* PAAGENTTEST_H_ */
