/*
 * Configuration.h
 *
 *  Created on: Sep 4, 2009
 *      Author: vasile
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#include <string>
#include <vector>
#include "Event.h"
#include "Action.h"
#include "CalendarEvent.h"
#include "IdlenessEvent.h"
#include "RMAction.h"
#include "P2PAction.h"
#include "CustomAction.h"
#include "AdvertAction.h"
using namespace std;
class Configuration {
private:
	string proactive_location;
	string java_home;
	string jvm_params;
	bool enable_memory_management;
	int java_memory;
	int native_memory;
	int no_processes;
	bool use_all_cpus;
	string config_protocol;
	int port_initial_value;
	//vector of events
	vector<Event> events;
	//vector of actions
	vector<Action> actions;
//	int onRuntimeExitScript;
	static Configuration* instance;
	//responsible with the configuration singleton
	//FIXME add default values
	void Initialize();
protected:
	Configuration();
public:
	virtual ~Configuration();
	static Configuration* Inst();
    vector<Event> getEvents() const
    {
        return events;
    }

    void setEvents(vector<Event> events)
    {
        this->events = events;
    }

    vector<Action> getActions() const
    {
        return actions;
    }

    void setActions(vector<Action> actions)
    {
        this->actions = actions;
    }
    //getters/setters
    string getProactive_location() const
    {
        return proactive_location;
    }

    void setProactive_location(string proactive_location)
    {
        this->proactive_location = proactive_location;
    }

    string getJava_home() const
    {
        return java_home;
    }

    void setJava_home(string java_home)
    {
        this->java_home = java_home;
    }

    string getJvm_params() const
    {
        return jvm_params;
    }

    void setJvm_params(string jvm_params)
    {
        this->jvm_params = jvm_params;
    }

    bool getEnable_memory_management() const
    {
        return enable_memory_management;
    }

    void setEnable_memory_management(bool enable_memory_management)
    {
        this->enable_memory_management = enable_memory_management;
    }

    int getJava_memory() const
    {
        return java_memory;
    }

    void setJava_memory(int java_memory)
    {
        this->java_memory = java_memory;
    }

    int getNative_memory() const
    {
        return native_memory;
    }

    void setNative_memory(int native_memory)
    {
        this->native_memory = native_memory;
    }

    int getNo_processes() const
    {
        return no_processes;
    }

    void setNo_processes(int no_processes)
    {
        this->no_processes = no_processes;
    }

    bool getUse_all_cpus() const
    {
        return use_all_cpus;
    }

    void setUse_all_cpus(bool use_all_cpus)
    {
        this->use_all_cpus = use_all_cpus;
    }

    string getConfig_protocol() const
    {
        return config_protocol;
    }

    void setConfig_protocol(string config_protocol)
    {
        this->config_protocol = config_protocol;
    }

    int getPort_initial_value() const
    {
        return port_initial_value;
    }

    void setPort_initial_value(int port_initial_value)
    {
        this->port_initial_value = port_initial_value;
    }

};

#endif /* CONFIGURATION_H_ */
