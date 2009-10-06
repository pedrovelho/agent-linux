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
 * ConfigHandlerTest.cpp
 *
 *  Created on: Sep 7, 2009
 *      Author: vasile
 */

#include "ConfigParserTest.h"
ConfigParserTest::ConfigParserTest() {
	// TODO Auto-generated constructor stub

}

ConfigParserTest::~ConfigParserTest() {
	// TODO Auto-generated destructor stub
}

void ConfigParserTest::SetUp() {
}
void ConfigParserTest::TearDown() {
}
/* load known test files and compare the objects
 * created
 */
TEST_F(ConfigParserTest, parse)
{

	ConfigParser parser("./PAAgent-config.xml");
	Configuration *config = parser.GetConfiguration();

	//test configuration values
	EXPECT_EQ("/home/vasile/workspacePA/PA_new/trunk", config->GetPALocation());

	EXPECT_EQ("/usr/lib/jvm/java-6-openjdk", config->GetJavaHome());
	EXPECT_EQ("Xmx 256", config->GetJVMParams());
	EXPECT_EQ(false, config->isMemoryManaged());
	EXPECT_EQ(0, config->GetJavaMemory());
	EXPECT_EQ(0, config->GetNativeMemory());
	EXPECT_EQ(false, config->UseAllCPUs());
	EXPECT_EQ("rmi", config->GetConfigProtocol());

	EXPECT_EQ(1, config->GetNoProcesses());
	EXPECT_EQ(1099, config->GetPortInitialValue());

	//	    ASSERT_GT()
	//test events
	vector<CalendarEvent*> events = config->GetCalendarEvents();
	ASSERT_EQ(1, events.size());
	CalendarEvent *cal1 = events.at(0);
	EXPECT_EQ(100,cal1->GetMaxCPUUsage());
	EXPECT_EQ("monday", cal1->GetStartDay());
	EXPECT_EQ(2,cal1->GetStartHour());
	EXPECT_EQ(3, cal1->GetStartMinute());
	EXPECT_EQ(23, cal1->GetStartSecond());
	EXPECT_EQ(6, cal1->GetDurationDays());
	EXPECT_EQ(23, cal1->GetDurationHours());
	EXPECT_EQ(59, cal1->GetDurationMinutes());
	EXPECT_EQ(59, cal1->GetDurationSeconds());
	EXPECT_EQ("Normal", cal1->GetProcessPriority());

	vector<IdlenessEvent*> idle_events = config->GetIdleEvents();
	ASSERT_EQ(1, idle_events.size());
	IdlenessEvent *idle1 = idle_events.at(0);
	EXPECT_EQ(10, idle1->GetBeginSeconds());
	EXPECT_EQ(50, idle1->GetEndSeconds());
	EXPECT_EQ(50, idle1->GetBeginThreshold());
	EXPECT_EQ(60, idle1->GetEndThreshold());

	//test actions
	vector<AdvertAction*> advert_actions = config->GetAdvertActions();
	ASSERT_EQ(1, advert_actions.size());
	AdvertAction *adaction = advert_actions.at(0);
	EXPECT_EQ(4, adaction->GetRestartDelay());
	EXPECT_EQ("org.objectweb.proactive.core.util.winagent.PAAgentServiceRMIStarter", adaction->GetStarterClass());
	EXPECT_EQ(false, adaction->IsEnabled());
	//	    EXPECT_EQ("", adaction->GetNodeName());

	vector<RMAction*> rm_actions = config->GetRMActions();
	ASSERT_EQ(1, rm_actions.size());
	RMAction *rmaction = rm_actions.at(0);
	EXPECT_EQ(0, rmaction->GetRestartDelay());
	EXPECT_EQ("org.ow2.proactive.resourcemanager.utils.PAAgentServiceRMStarter", rmaction->GetStarterClass());
	EXPECT_EQ(true, rmaction->IsEnabled());
	EXPECT_EQ("ilie", rmaction->GetUsername());
	EXPECT_EQ("delascularie", rmaction->GetPassword());
	//	    EXPECT_EQ("", rmaction->GetURL());
	EXPECT_EQ("nod_in_gat", rmaction->GetNodeName());

	vector<P2PAction*> p2p_actions = config->GetP2PActions();
	ASSERT_EQ(1, p2p_actions.size());
	P2PAction *p2paction = p2p_actions.at(0);
	EXPECT_EQ(100, p2paction->GetRestartDelay());
	EXPECT_EQ("ilie@scularie", p2paction->GetContact());
	EXPECT_EQ("RMI", p2paction->GetProtocol());
	EXPECT_EQ("p2p_node",p2paction->GetNodeName());
	EXPECT_EQ(false, p2paction->IsEnabled());
	EXPECT_EQ("P2PStarter",p2paction->GetStarterClass());

	vector<CustomAction*> cust_actions = config->GetCustomActions();
	ASSERT_EQ(1, cust_actions.size());
	CustomAction *custaction = cust_actions.at(0);
	EXPECT_EQ(0, custaction->GetRestartDelay());
	EXPECT_EQ("some arguments", custaction->GetArguments());
	EXPECT_EQ("custom",custaction->GetNodeName());
	EXPECT_EQ(false, custaction->IsEnabled());
	EXPECT_EQ("user.Starter",custaction->GetStarterClass());
}