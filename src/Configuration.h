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
 * Configuration.h
 *
 *  Created on: Sep 4, 2009
 *      Author: vasile
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include "Event.h"
#include "Action.h"
#include "CalendarEvent.h"
#include "IdlenessEvent.h"
#include "RMAction.h"
#include "P2PAction.h"
#include "CustomAction.h"
#include "AdvertAction.h"
#include "Constants.h"
using namespace paagent::constants;
using namespace std;
namespace paagent {
/**
 * Singleton container class for configuration read from the
 * XML file.
 * */
class Configuration {
private:
	/** java policy file */
	string security_policy;
	/** log4j configuration file */
	string log4j_file;
	/** Location of ProActive used in starting nodes */
	string proactive_location;
	string java_home;
	/** General parameters for all JVMs started*/
	vector<string> jvm_params;
	/** Flag for enabling Java and native memory checks*/
	bool enable_memory_management;
	/** Maximum Java memory to be used */
	int java_memory;
	/** Maximum native memory to be used */
	int native_memory;
	/** Maxim number of processes */
	int no_processes;
	bool use_all_cpus;
	string config_protocol;
	string classpath;
	string java_bin;
	unsigned int port_initial_value;
	vector<CalendarEvent*> calendar_events;
	vector<IdlenessEvent*> idle_events;
	vector<P2PAction*> p2p_actions;
	vector<RMAction*> rm_actions;
	vector<AdvertAction*> advert_actions;
	vector<CustomAction*> custom_actions;
	static Configuration *instance;
	void Initialize();
protected:
	Configuration();
public:
	/** Gets the number of CPUs on the machine. It is used
	 * in calculating the idleness of the processors
	 *
	 * @return the number of CPUs
	 * */
	int GetNoCPUS();
	/** Gets the ProActive classpath.
	 * @return a path with the ProActive jars defined in Constants.h
	 * */
	string GetClasspath();
	/** Adds an Action to one of the Action vectors. The Action
	 * is added to the vectors depending on the type.
	 *
	 * @param action a pointer to an Action
	 * */
	void AddAction(Action* action);
	/** Adds an Event to one of the Event vectors. The Event
	 * is added to the vectors depending on the type.
	 *
	 * @param event a pointer to an Event
	 * */
	void AddEvent(Event* event);
	virtual ~Configuration();
	/** Singleton initialization method. */
	static Configuration *Inst();

	vector<CalendarEvent*> GetCalendarEvents() const;

	void SetCalendarEvents(vector<CalendarEvent*> calendar_events);

	vector<IdlenessEvent*> GetIdleEvents() const;

	void SetIdleEvents(vector<IdlenessEvent*> idle_events);

	vector<P2PAction*> GetP2PActions() const;

	void SetP2Actions(vector<P2PAction*> p2p_actions);

	vector<RMAction*> GetRMActions() const;

	void SetRMActions(vector<RMAction*> rm_actions);

	vector<AdvertAction*> GetAdvertActions() const;

	void SetAdvertActions(vector<AdvertAction*> advert_actions);

	vector<CustomAction*> GetCustomActions() const;

	void SetCustomActions(vector<CustomAction*> custom_actions);

	string GetPALocation() const;

	void SetPALocation(string proactive_location);

	string GetJavaHome() const;

	void SetJavaHome(string java_home);

	vector<string> GetJVMParams() const;
	void SetJVMParams(string jvm_params);

	bool IsMemoryManaged() const;
	void SetMemoryManagement(bool enable_memory_management);

	int GetJavaMemory() const;

	void SetJavaMemory(int java_memory);

	int GetNativeMemory() const;

	void SetNativeMemory(int native_memory);

	int GetNoProcesses() const;

	void SetNoProcesses(int no_processes);

	bool UseAllCPUs() const;

	void SetUseAllCPUs(bool use_all_cpus);

	string GetConfigProtocol() const;

	void SetConfigProtocol(string config_protocol);

	int GetPortInitialValue() const;

	void SetPortInitialValue(int port_initial_value);
	void SetJavaSecurityPolicy(string policy);
	string GetJavaSecurityPolicy() const;
	void SetLog4jFile(string log4j);
	string GetLog4jFile() const;
	void SetClasspath(string path);
	string GetClasspath() const;
	void SetJavaBin(string bin);
	string GetJavaBin() const;

};
} //namespace paagent
#endif /* CONFIGURATION_H_ */
