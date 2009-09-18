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
const string DEFAULT_JAVA_CLASS("org.objectweb.proactive.core.node.StartNode");

//JAVACMD=$JAVA_HOME"/bin/java -Djava.security.manager -Djava.security.policy=$PROACTIVE/examples/proactive.java.policy -Dlog4j.configuration=file:${PROACTIVE}/examples/proactive-log4j -Dproactive.home=$PROACTIVE "
const string DEFAULT_JAVA_BIN(DEFAULT_JAVA_HOME + "/bin/java");
const string DEFAULT_DSECURITY_MANAGER("-Djava.security.manager");
const string DEFAULT_DJAVA_SECURITY("-Djava.security.policy="+ DEFAULT_PROACTIVE_HOME + "/examples/proactive.java.policy");
const string DEFAULT_DLOG4J_FILE("-Dlog4j.configuration=file:" + DEFAULT_PROACTIVE_HOME + "/examples/proactive-log4j");
const string DEFAULT_DPROACTIVE_HOME("-Dproactive.home="+ DEFAULT_PROACTIVE_HOME);
const string PROACTIVE_JAR("/dist/lib/ProActive.jar");
const string PROACTIVE_EXAMPLES_JAR("/dist/lib/ProActive_examples.jar");
const string PROACTIVE_UTILS_JAR("/dist/lib/ProActive_utils.jar");
//CLASSPATH=$CLASSPATH:$PROACTIVE/dist/lib/ProActive.jar
//CLASSPATH=$CLASSPATH:$PROACTIVE/dist/lib/ProActive_examples.jar
//CLASSPATH=$CLASSPATH:$PROACTIVE/dist/lib/ProActive_utils.jar
const string DEFAULT_DCLASSPATH(DEFAULT_PROACTIVE_HOME
		+ PROACTIVE_JAR+ ":" + DEFAULT_PROACTIVE_HOME
		+ PROACTIVE_EXAMPLES_JAR + ":" + DEFAULT_PROACTIVE_HOME
		+ PROACTIVE_UTILS_JAR);

//const string DEFAULT_NODE_STARTER(DEFAULT_JAVA_COMMAND + " "
//		+" -classpath " + DEFAULT_CLASSPATH + " " + DEFAULT_JAVA_CLASS );

const string JVM_STOPPED("JVM_STOPPED");
//the JVM kill needs some time to finish, setting an arbitray time here to wait for the JVM to stop
const int SLEEP_TIME_AFTER_KILL = 3;
const int DEFAULT_TICK = 1000;

//define types of actions


//used in watchers to decide what action to restart
enum ActionType {ADVERT, RM, CUSTOM, P2P};

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
