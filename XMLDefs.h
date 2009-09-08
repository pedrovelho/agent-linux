/*
 * ConfigHandler.h
 *
 *  Created on: Apr 28, 2009
 *      Author: vasile
 */

#ifndef XMLDEFS_H_
#define XMLDEFS_H_
#include <string>
using namespace std;
namespace xmltags {
//XML elements
//configuration
const string PROACTIVE_LOCATION("proactiveLocation");
const string JAVA_HOME("java_home");
const string JAVA_PARAMS("jvm_params");
const string ENABLE_MEMORY_MNGMT("enable_memory_management");
const string JAVA_MEMORY("java_memory");
const string NATIVE_MEMORY("native_memory");
const string NO_PROCESSES("nb_processes");
const string USE_ALL_CPUS("use_all_cpus");
const string CONFIG_PROTOCOL("protocol");
const string PORT_INITIAL_VALUE("port_initial_value");
const string ON_RT_EXIT_SCRIPT("onRuntimeExitScript");

const string ENABLED("true");
const string DISABLED("false");

const string EVENT("event");
const string NODE_NAME("nodeName");
const string ACTION("action");
const string AGENT("agent"); //xml root
const string INTERNAL_CONFIG("internalConfig");
const string EVENTS("events");
const string ACTIONS("actions");
const string BEGIN_SECONDS("beginSecs");
const string END_SECONDS("endSecs");
const string BEGIN_THRESHOLD("beginThreshold");
const string END_THRESHOLD("endThreshold");
const string START_DAY("startDay");
const string START_HOUR("startHour");
const string START_MINUTE("startMinute");
const string START_SECOND("startSecond");
const string DURATION_DAYS("durationDays");
const string DURATION_HOURS("durationHours");
const string DURATION_MINUTES("durationMinutes");
const string DURATION_SECONDS("durationSeconds");
const string PROCESS_PRIORITY("processPriority");
const string MAX_CPU_USAGE("maxCpuUsage");
const string MONDAY("monday");
const string TUESDAY("tuesday");
const string WEDNESDAY("wednesday");
const string THURSDAY("thursday");
const string FRIDAY("friday");
const string SATURDAY("saturday");
const string SUNDAY("sunday");

//action types
const string ADVERT_ACTION("AdvertAction");
const string RM_ACTION("RMAction");
const string CUSTOM_ACTION("CustomAction");
const string P2P_ACTION("P2PAction");

//event types
const string CALENDAR_EVENT("CalendarEvent");
const string IDLENESS_EVENT("IdlenessEvent");

const string INITIAL_RESTART_DELAY("initialRestartDelay");
const string JAVA_STARTER_CLASS("javaStarterClass");
const string IS_ENABLED("isEnabled");
const string ACTION_ARGS("args");
const string USERNAME("username");
const string PASSWORD("password");
const string PROTOCOL("protocol");
const string CONTACT("contact");
const string URL("url");
}
#endif /* XMLDEFS_H_ */
