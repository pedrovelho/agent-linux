/*
 * ControllerTest.h
 *
 *  Created on: Sep 3, 2009
 *      Author: vasile
 */

#ifndef CONTROLLERTEST_H_
#define CONTROLLERTEST_H_

#include <gtest/gtest.h>
#include "Controller.h"

class ControllerTest : public ::testing::Test {
public:
	ControllerTest();
	virtual ~ControllerTest();
protected:
	int first_pid;
	int second_pid;
	int third_pid;
 virtual void SetUp();
 //stops all JVMS
 virtual void TearDown();
};
#endif /* CONTROLLERTEST_H_ */
