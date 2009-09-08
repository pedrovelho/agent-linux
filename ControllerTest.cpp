/*
 * ControllerTest.cpp
 *
 *  Created on: Sep 3, 2009
 *      Author: vasile
 */

#include "ControllerTest.h"

ControllerTest::ControllerTest() {
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
	cout << "Stopping JVMs started by tests..." << endl;
	kill(first_pid, 15);
	kill(second_pid, 15);
	kill(third_pid, 15);
}
/* Try starting a few nodes using the Controller class
 and check they have been started.
 */
TEST_F(ControllerTest, StartNode)
{
	//start a few nodes and check if they are alive using kill
	//it will check for the *shell* pid not the JVM pid
	Controller controller;

	first_pid = controller.StartNode(DEFAULT_SHELL, DEFAULT_NODE_EXEC,
			"first_node");
	EXPECT_NE(-1, kill(first_pid, 0));
	second_pid = controller.StartNode(DEFAULT_SHELL, DEFAULT_NODE_EXEC, "second_node");
	EXPECT_NE(-1, kill(second_pid, 0));
	third_pid = controller.StartNode(DEFAULT_SHELL, DEFAULT_NODE_EXEC, "third_node");
	EXPECT_NE(-1, kill(third_pid, 0));
}

TEST_F(ControllerTest, StopNode)
{
	//start a few nodes and check if they are alive using kill
	//it will check for the *shell* pid not the JVM pid
	Controller controller;
	first_pid = controller.StartNode(DEFAULT_SHELL, DEFAULT_NODE_EXEC,
			"first_node");
	ASSERT_NE(-1, kill(first_pid, 0));

	second_pid = controller.StartNode(DEFAULT_SHELL, DEFAULT_NODE_EXEC, "second_node");
	ASSERT_NE(-1, kill(second_pid, 0));

	third_pid = controller.StartNode(DEFAULT_SHELL, DEFAULT_NODE_EXEC, "third_node");
	ASSERT_NE(-1, kill(third_pid, 0));
	//try to stop the  JVMS and check they are stopped

	bool dead_jim = controller.StopNode(first_pid);
	EXPECT_EQ(-1, kill(first_pid, 0));
	EXPECT_EQ(true, dead_jim);

	dead_jim = controller.StopNode(second_pid);
	EXPECT_EQ(-1, kill(second_pid, 0));
	EXPECT_EQ(true, dead_jim);

//	dead_jim = controller.StopNode(third_pid);
//	EXPECT_EQ(-1, kill(third_pid, 0));
//	EXPECT_EQ(true, dead_jim);
}

