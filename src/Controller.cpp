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
 * Controller.cpp
 *
 *  Created on: Mar 19, 2009
 *      Author: vasile
 */

#include "Controller.h"
namespace paagent {
Controller::Controller() {
	logger = log4cxx::Logger::getLogger("Controller");
}
//Controller::Controller(const Controller &controller){
//	default_node_exec = controller.default_node_exec;
//	default_shell = controller.default_shell;
////	logger =  log4cxx::Logger::getLogger("Controller");
//}
Controller::~Controller() {
}
int Controller::StartNode(string name, string java_class) {
	NodeStarter starter(name, java_class);
	starter.run();
	LOG4CXX_INFO(logger, "Regular node started");
	return starter.getPid();
}

int Controller::StartRMNode(string name, string java_class, string user,
		string password, string url) {

	RMNodeStarter starter(name, java_class, user, password, url);
	starter.run();
	LOG4CXX_INFO(logger, "Resource Manager node started");
	return starter.getPid();
}
int Controller::StartP2PNode(string name, string java_class, string contact) {
	P2PNodeStarter starter(name, java_class, contact);
	starter.run();
	LOG4CXX_INFO(logger, "P2P node started");
	return starter.getPid();
}

int Controller::StartCustomNode(string name, string java_class,
		string arguments) {
	CustomNodeStarter starter(name, java_class, arguments);
	starter.run();
	LOG4CXX_INFO(logger, "Custom node started");
	return starter.getPid();
}

bool Controller::StopNode(int pid) {
	//try to stop

	kill(pid, SIGTERM);
	//wait for the JVM to stop
	sleep(SLEEP_TIME_AFTER_KILL);
	bool dead = false;
	//return check for stop,
	//kill(pid, 0) returns -1 if the process does not exist
	int stop = kill(pid, 0);
	LOG4CXX_INFO(logger, "Sent SIGTERM message to node with PID " << pid);
	if (stop != 0) {
		dead = true;
	}
	return dead;
}

bool Controller::StopNode(string name) {
	LOG4CXX_ERROR(logger,"Method not implemented, use stop by PID");
	return false;
}
/******
 * Necessary kludge because of dbus-cxx limitations
 *
 * All 3 methods should be called to set a proper start
 * configuration for JVMs
 *
 * This should go away as soon as dbus-cxx has support for
 * passing arrays/vectors (v0.5 has some basic support already)
 *******/
void Controller::SetStartConfiguration(string security_policy,
		string log4j_file, string proactive_home, string classpath,
		string java_bin) {
	Configuration *config = Configuration::Inst();
	config->SetJavaSecurityPolicy(security_policy);
	config->SetLog4jFile(log4j_file);
	config->SetPALocation(proactive_home);
	config->SetClasspath(classpath);
	config->SetJavaBin(java_bin);
	LOG4CXX_INFO(logger, "Start configuration set");
}

void Controller::SetJVMSettings(string jvm_params, bool enable_mem_mngmnt,
		int java_memory, int native_memory, int no_processes, bool use_all_cpus) {
	Configuration *config = Configuration::Inst();
	config->SetJVMParams(jvm_params);
	config->SetMemoryManagement(enable_mem_mngmnt);
	config->SetJavaMemory(java_memory);
	config->SetNativeMemory(native_memory);
	config->SetNoProcesses(no_processes);
	config->SetUseAllCPUs(use_all_cpus);
	LOG4CXX_INFO(logger, "JVM settings set");
}
void Controller::SetNetworkSettings(int port_value, string protocol) {
	Configuration *config = Configuration::Inst();
	config->SetConfigProtocol(protocol);
	config->SetPortInitialValue(port_value);
	LOG4CXX_INFO(logger, "Network settings set");
}
} //namespace paagent
