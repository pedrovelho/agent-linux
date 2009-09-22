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
	//	::testing::InitGoogleTest(&argc, argv);
	//	return RUN_ALL_TESTS();
	//

	DBus::init();
	DBus::Dispatcher dispatcher;
	DBus::Connection::pointer connection = dispatcher.create_connection(
			DBus::BUS_SESSION);
	DBus::ControllerProxy::pointer controller = DBus::ControllerProxy::create(
			connection);
	//	for(;;){
	//	cout << controller->StartNode("unuDetest") <<  endl;;
	//	usleep(1000*1000);
	//	}
}
