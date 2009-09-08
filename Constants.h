/*
 * Constants.h
 *
 *  Created on: Aug 11, 2009
 *      Author: vasile
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <string>
using namespace std;
const string PROACTIVE_DBUS_SERVER("pa.controller");
const string PROACTIVE_DBUS_WATCHER("pa.watcher");
//define sleep seconds for checking signals loop
const int LISTEN_SIGNAL_TIMEOUT = 1;
const int TICK_DEFAULT = 1000;
const string DEFAULT_SHELL("/bin/bash");
const string DEFAULT_NODE_EXEC(
		"/home/vasile/workspacePA/PA_new/trunk/bin/startNode.sh");

const string DEFAULT_NODE("default_node");
const string DEFAULT_PROACTIVE_HOME("/home/vasile/workspacePA/PA_new/trunk");
const string DEFAULT_JAVA_HOME("/usr/lib/jvm/java-6-openjdk");

const string JVM_STOPPED("JVM_STOPPED");

#define PROACTIVE_SERVICE_NAME        "pa.agent"
/* Object path to the provided object. */
#define PROACTIVE_SERVICE_OBJECT_PATH "pa/agent/controller"
/* And we're interested in using it through this interface.
 This must match the entry in the interface definition XML. */
#define PROACTIVE_SERVICE_INTERFACE   "pa.agent.controller"
/* Symbolic constants for the signal names to use with GLib.
 These need to map into the D-Bus signal names. */
#define SIGNAL_CHANGED_VALUE1    "changed_value1"
#define SIGNAL_CHANGED_VALUE2    "changed_value2"
#define SIGNAL_OUTOFRANGE_VALUE1 "outofrange_value1"
#define SIGNAL_OUTOFRANGE_VALUE2 "outofrange_value2"

#endif /* CONSTANTS_H_ */
