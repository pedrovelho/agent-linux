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
 * Configuration.cpp
 *
 *  Created on: Sep 4, 2009
 *      Author: vasile
 */

#include "Configuration.h"
#include <iostream>
namespace paagent {
Configuration* Configuration::instance = NULL;

Configuration* Configuration::Inst() {
	if (instance == NULL) {
		instance = new Configuration();
	}
	return instance;
}

//private constructor
Configuration::Configuration() {
	Initialize();
}

Configuration::~Configuration() {
	if (instance != 0)
		delete instance;
}
void Configuration::Initialize() {
}
//FIXME dekludgeme introspect much ?
void Configuration::AddAction(Action *action) {

	string action_type = typeid(*action).name();
	if (action_type == typeid(RMAction).name()) {
		rm_actions.push_back(static_cast<RMAction*> (action));
	} else if (action_type == typeid(P2PAction).name()) {
		p2p_actions.push_back(static_cast<P2PAction*> (action));
	} else if (action_type == typeid(CustomAction).name()) {
		custom_actions.push_back(static_cast<CustomAction*> (action));
	} else if (action_type == typeid(AdvertAction).name()) {
		advert_actions.push_back(static_cast<AdvertAction*> (action));
	}
}
void Configuration::AddEvent(Event *event) {
	string event_type = typeid(*event).name();
	if (event_type == typeid(CalendarEvent).name()) {
		calendar_events.push_back(static_cast<CalendarEvent*> (event));
	} else if (event_type == typeid(IdlenessEvent).name()) {
		idle_events.push_back(static_cast<IdlenessEvent*> (event));
	}
}
int Configuration::GetNoCPUS() {
	return sysconf(_SC_NPROCESSORS_ONLN);
}
string Configuration::GetClasspath() {

	string classpath;
	classpath = proactive_location + PROACTIVE_JAR + ":" + proactive_location
			+ PROACTIVE_EXAMPLES_JAR + ":" + proactive_location
			+ PROACTIVE_UTILS_JAR;
	return classpath;
}

vector<CalendarEvent*> Configuration::GetCalendarEvents() const {
	return calendar_events;
}

void Configuration::SetCalendarEvents(vector<CalendarEvent*> calendar_events) {
	this->calendar_events = calendar_events;
}

vector<IdlenessEvent*> Configuration::GetIdleEvents() const {
	return idle_events;
}

void Configuration::SetIdleEvents(vector<IdlenessEvent*> idle_events) {
	this->idle_events = idle_events;
}

vector<P2PAction*> Configuration::GetP2PActions() const {
	return p2p_actions;
}

void Configuration::SetP2Actions(vector<P2PAction*> p2p_actions) {
	this->p2p_actions = p2p_actions;
}

vector<RMAction*> Configuration::GetRMActions() const {
	return rm_actions;
}

void Configuration::SetRMActions(vector<RMAction*> rm_actions) {
	this->rm_actions = rm_actions;
}

vector<AdvertAction*> Configuration::GetAdvertActions() const {
	return advert_actions;
}

void Configuration::SetAdvertActions(vector<AdvertAction*> advert_actions) {
	this->advert_actions = advert_actions;
}

vector<CustomAction*> Configuration::GetCustomActions() const {
	return custom_actions;
}

void Configuration::SetCustomActions(vector<CustomAction*> custom_actions) {
	this->custom_actions = custom_actions;
}

string Configuration::GetPALocation() const {
	return proactive_location;
}

void Configuration::SetPALocation(string proactive_location) {
	this->proactive_location = proactive_location;
}

string Configuration::GetJavaHome() const {
	return java_home;
}

void Configuration::SetJavaHome(string java_home) {
	this->java_home = java_home;
}

vector<string> Configuration::GetJVMParams() const {
	return jvm_params;
}

string Configuration::GetJVMParamsString() const {
	return jvm_params_string;
}

void Configuration::SetJVMParams(string jvm_params) {
	//clear vector
	this->jvm_params_string = jvm_params;
	this->jvm_params.empty();
	//separate arguments
	istringstream iss(jvm_params);
	copy(istream_iterator<string> (iss), istream_iterator<string> (),
			back_inserter<vector<string> > (this->jvm_params));
}

bool Configuration::IsMemoryManaged() const {
	return enable_memory_management;
}

void Configuration::SetMemoryManagement(bool enable_memory_management) {
	this->enable_memory_management = enable_memory_management;
}

int Configuration::GetJavaMemory() const {
	return java_memory;
}

void Configuration::SetJavaMemory(int java_memory) {
	this->java_memory = java_memory;
}

int Configuration::GetNativeMemory() const {
	return native_memory;
}

void Configuration::SetNativeMemory(int native_memory) {
	this->native_memory = native_memory;
}

int Configuration::GetNoProcesses() const {
	return no_processes;
}

void Configuration::SetNoProcesses(int no_processes) {
	this->no_processes = no_processes;
}

bool Configuration::UseAllCPUs() const {
	return use_all_cpus;
}

void Configuration::SetUseAllCPUs(bool use_all_cpus) {
	this->use_all_cpus = use_all_cpus;
}

string Configuration::GetConfigProtocol() const {
	return config_protocol;
}

void Configuration::SetConfigProtocol(string config_protocol) {
	this->config_protocol = config_protocol;
}

int Configuration::GetPortInitialValue() const {
	return port_initial_value;
}

void Configuration::SetPortInitialValue(int port_initial_value) {
	this->port_initial_value = port_initial_value;
}
void Configuration::SetJavaSecurityPolicy(string policy) {
	this->security_policy = policy;
}
string Configuration::GetJavaSecurityPolicy() const {
	return security_policy;
}
void Configuration::SetLog4jFile(string log4j) {
	this->log4j_file = log4j;
}
string Configuration::GetLog4jFile() const {
	return log4j_file;
}
void Configuration::SetClasspath(string path) {
	this->classpath = path;
}
string Configuration::GetClasspath() const {
	return classpath;
}
void Configuration::SetJavaBin(string bin) {
	this->java_bin = bin;
}
string Configuration::GetJavaBin() const {
	return java_bin;
}

} //namespace paagent

//static int get_ncpu() {
//	int ncpu = -1;
//#ifdef _SC_NPROCESSORS_ONLN
//	ncpu = sysconf(_SC_NPROCESSORS_ONLN);
//#elif defined __APPLE__
//	int mib[2] = {CTL_HW, HW_NCPU};
//	size_t len = sizeof(ncpu);
//	sysctl(mib, 2, &ncpu, &len, NULL, 0);
//#endif
//	return ncpu;
//}
