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
 * Constants.h
 *
 *  Created on: Aug 11, 2009
 *      Author: vasile
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <string>
using namespace std;
/**
 * Constants and defaults used by the client and the server.
 */
namespace pa_agent_constants {
/** The name the agent requests on DBus. This is the same name as the
 * one defined in Controller.xml for the dest attribute. */
const string PROACTIVE_DBUS_NAME("proactive.agent.controller");

/** Default class used to start JVMs. To customize the class, use the
 * configuration XML file  and set the javaStarterClass element  in Action elements. */
const string DEFAULT_JAVA_CLASS("org.objectweb.proactive.core.node.StartNode");

//JAVACMD=$JAVA_HOME"/bin/java -Djava.security.manager -Djava.security.policy=$PROACTIVE/examples/proactive.java.policy -Dlog4j.configuration=file:${PROACTIVE}/examples/proactive-log4j -Dproactive.home=$PROACTIVE "
/** Default Java executable */
const string DEFAULT_JAVA_BIN("/bin/java");

/** This is the option name for enabling the security manager in Java. */
const string DEFAULT_DSECURITY_MANAGER("-Djava.security.manager");

/** Default option name for setting a security policy file in Java */
const string DEFAULT_DJAVA_SECURITY_OPTION("-Djava.security.policy=");
/** Default security policy file path. */
const string DEFAULT_DJAVA_SECURITY_FILE("/examples/proactive.java.policy");

/** Default option for setting a log4j configuration file. */
const string DEFAULT_DLOG4J_OPTION("-Dlog4j.configuration=file:");
/** Default log4j configuration file path for nodes */
const string DEFAULT_DLOG4J_FILE("/examples/proactive-log4j");

/** Default option name for setting the ProActive home*/
const string DEFAULT_DPROACTIVE_OPTION("-Dproactive.home=");

//CLASSPATH=$CLASSPATH:$PROACTIVE/dist/lib/ProActive.jar
//CLASSPATH=$CLASSPATH:$PROACTIVE/dist/lib/ProActive_examples.jar
//CLASSPATH=$CLASSPATH:$PROACTIVE/dist/lib/ProActive_utils.jar

/** Default name of the main ProActive jar file. */
const string PROACTIVE_JAR("/dist/lib/ProActive.jar");
/** Default name for the ProActive examples jar file. */
const string PROACTIVE_EXAMPLES_JAR("/dist/lib/ProActive_examples.jar");
/** Default name for the ProActive utils jar file. */
const string PROACTIVE_UTILS_JAR("/dist/lib/ProActive_utils.jar");

/** Time to wait for the JVM to stop when stopping nodes. */
const int SLEEP_TIME_AFTER_KILL = 5;
/** Time interval between node checks for watchers. */
const int DEFAULT_TICK = 1000;

/** Enum identifying types of Actions.
 * It is used in {@link Watcher} to decide which action to restart.
 */
enum ActionType {
	ADVERT, RM, CUSTOM, P2P
};
}
#endif /* CONSTANTS_H_ */
