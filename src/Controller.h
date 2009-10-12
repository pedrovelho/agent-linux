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
 * Controller.h
 *
 *  Created on: Mar 19, 2009
 *      Author: vasile
 */

#ifndef GENERICS
#define GENERICS

#endif

#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

#include <signal.h>
#include "RMNodeStarter.h"
#include "CustomNodeStarter.h"
#include "P2PNodeStarter.h"
#include "NodeStarter.h"
#include "Constants.h"
#include "Configuration.h"
using namespace paagent::constants;
#include <sigc++-2.0/sigc++/sigc++.h>
#include <string>
#include <cstdint>
namespace paagent {
class Controller {
private:
	LoggerPtr logger;
public:
	void SendSignal(const string signal, string node_name);
	Controller();
	Controller(const Controller &controller);
	virtual ~Controller();
	/**
	 * Starts a JVM with the default node starter and shell.
	 * shell - the full path to the Linux shell used to start
	 * 		the node executable
	 * node_executable - the node executable (bash script) used
	 * 		to start a JVM with the node
	 * 		usually:  PROACTIVE_FOLDER/bin/startNode.sh
	 * node_name - the name of the node to be started
	 *		can be in the form ibis://localdomain.localhost/node
	 *		in order to specify a protocol. See the ProActive manuals
	 *		for more information.
	 *
	 * Returns 0 if there is an error or the node_executable or
	 * the shell are not set.
	 * Returns the pid of the shell process which starts the Node
	 * on successful execution.
	 */
	int StartNode(string name, string java_class);
	/**
	 * Starts a JVM with the default node starter and shell.
	 * node_name - the name of the node to be started
	 *		can be in the form ibis://localdomain.localhost/node
	 *		in order to specify a protocol. See the ProActive manuals
	 *		for more information.
	 *
	 * Returns 0 if there is an error or the node_executable or
	 * the shell are not set.
	 * Returns the pid of the shell process which starts the Node
	 * on successful execution.
	 */
	int StartNode(string node_name);
	/**
	 * Stops the watcher and the Node assigned to the corresponding pid.
	 * pid - the pid of the process being watched
	 *
	 *
	 * Returns false if no Node has been found.
	 */
	int StartRMNode(string name, string java_class, string user,
			string password, string url);
	//contact ? WTF
	int StartP2PNode(string name, string java_class, string contact);
	int StartCustomNode(string name, string java_class, string arguments);
	bool StopNode(int pid);

	/**
	 * Stops the Node with the give name.
	 * node_name - the name of the node to stop
	 *		Should be given in the full form
	 *		i.e. ibis://localdomain.localhost/node
	 *		i.e. node1
	 */
	bool StopNode(string node_name);
	void SetStartConfiguration(string security_policy, string log4j_file,
			string proactive_home, string classpath, string java_bin);
	void SetJVMSettings(string jvm_params, bool enable_mem_mngmnt,
			int java_memory, int native_memory, int no_processes,
			bool use_all_cpus);
	void SetNetworkSettings(int port_value, string protocol);
};
}//namespace paagent
#endif /* CONTROLLER_H_ */

