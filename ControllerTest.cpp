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
 * ControllerTest.cpp
 *
 *  Created on: Sep 3, 2009
 *      Author: vasile
 */

#include "ControllerTest.h"

ControllerTest::ControllerTest() {
	logger = log4cxx::Logger::getLogger("ControllerTest");
}

ControllerTest::~ControllerTest() {
	// TODO Auto-generated destructor stub
}

void ControllerTest::SetUp() {
	first_pid = 0;
	second_pid = 0;
	third_pid = 0;
}
void ControllerTest::TearDown() {
	//kill started jvms
	LOG4CXX_DEBUG(logger, "Stopping JVMs started by tests...");
	//kill  bill I
	kill(first_pid, SIGTERM);
	kill(second_pid, SIGTERM);
	kill(third_pid, SIGTERM);
}
/* Try starting a few nodes using the Controller class
 and check they have been started.
 */
TEST_F(ControllerTest, StartNode)
{
	//start a few nodes and check if they are alive using kill
	//it will check for the *shell* pid not the JVM pid
	Controller controller;

	first_pid = controller.StartNode("first_node", DEFAULT_JAVA_CLASS);
	EXPECT_NE(-1, kill(first_pid, 0));
	second_pid = controller.StartNode("second_node", DEFAULT_JAVA_CLASS);
	EXPECT_NE(-1, kill(second_pid, 0));
	third_pid = controller.StartNode("third_node", DEFAULT_JAVA_CLASS);
	EXPECT_NE(-1, kill(third_pid, 0));

}

TEST_F(ControllerTest, StopNode)
{
	//start a few nodes and check if they are alive using kill
	//it will check for the *shell* pid not the JVM pid
	Controller controller;
	first_pid = controller.StartNode("first_node", DEFAULT_JAVA_CLASS);
	ASSERT_NE(-1, kill(first_pid, 0));

	second_pid = controller.StartNode("second_node",DEFAULT_JAVA_CLASS);
	ASSERT_NE(-1, kill(second_pid, 0));

	third_pid = controller.StartNode("third_node", DEFAULT_JAVA_CLASS);
	ASSERT_NE(-1, kill(third_pid, 0));
	//try to stop the  JVMS and check they are stopped

	//	bool dead_jim = controller.StopNode(first_pid);
	//	EXPECT_EQ(-1, kill(first_pid, 0));
	//	EXPECT_EQ(true, dead_jim);
	//
	//	dead_jim = controller.StopNode(second_pid);
	//	EXPECT_EQ(-1, kill(second_pid, 0));
	//	EXPECT_EQ(true, dead_jim);
	//
	//	dead_jim = controller.StopNode(third_pid);
	//	EXPECT_EQ(-1, kill(third_pid, 0));
	//	EXPECT_EQ(true, dead_jim);
}

