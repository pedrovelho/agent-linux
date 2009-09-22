/*
 * ################################################################
 *
 * ProActive: The ProActive Linux Agent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 *
 * ################################################################
 */

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

class ControllerTest: public ::testing::Test {
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
private:
	log4cxx::LoggerPtr logger;
};
#endif /* CONTROLLERTEST_H_ */
