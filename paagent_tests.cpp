/*
 * paagent-client.cpp
 *
 *  Created on: Sep 3, 2009
 *      Author: vasile
 */

#include <iostream>

#include <string>
//#include "PAAgent.h"
//using namespace std;
//int main() {
//
//	string shell = "/bin/bash";
//
//	string node_exec = "/home/vasile/workspacePA/PA_new/trunk/bin/startNode.sh";
//
//	string node_name = "node1";
//
//	PAAgent agent;
//	agent.start();
//	agent.join();
//}

#include <gtest.h>
#include "ControllerTest.h"
#include "PAAgentTest.h"
using namespace std;
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

}
