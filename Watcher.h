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
 * Watcher.h
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#ifndef GENERICS
#define GENERICS

#endif

#ifndef WATCHER_H_
#define WATCHER_H_
#include <iostream>
#include <signal.h>
#include <string>

//run  ccgnu2-config --help  for libraries and includes
#include <cc++/thread.h>
using namespace ost;

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

#include "controller_proxy.h"
#include "Constants.h"
using namespace pa_agent_constants;
class Watcher: public Thread {
private:
	log4cxx::LoggerPtr logger;
	int pid;
	string node_name;
	int tick;
	bool stop;
	long restart_delay;
	DBus::ControllerProxy::pointer controller;
	ActionType action_select;
	string user;
	string password;
	string url;
	string contact;
	string arguments;
	string java_class;
public:

	//named constructors for different types of action restarts
	//AdvertAction
	static Watcher* AdvertWatcher(int jvm_pid, int tick, int restart_delay,
			string name, string java_class,
			DBus::ControllerProxy::pointer controller);
	//RMAction
	static Watcher* RMWatcher(int jvm_pid, int tick, int restart_delay,
			string name, string java_class, string user, string password,
			string url, DBus::ControllerProxy::pointer controller);
	//P2PAction
	static Watcher* P2PWatcher(int jvm_pid, int tick, int restart_delay,
			string name, string java_class, string contact,
			DBus::ControllerProxy::pointer controller);
	//CustomAction
	static Watcher* CustomWatcher(int jvm_pid, int tick, int restart_delay,
			string name, string java_class, string arguments,
			DBus::ControllerProxy::pointer controller);

	int GetTick();
	int GetPid();
	//	void SetTick();
	void SetPid();
	long GetRestartDelay();
	void SetRestartDelay();
	void StopWatcher();
	virtual ~Watcher();
	void run();
	Watcher(int jvm_pid, int tick, int restart_delay, string name,
			string java_class, DBus::ControllerProxy::pointer controller,
			ActionType action);
	//these should be private
	void SetRMValues(string user, string password, string url);
	void SetP2PValues(string contact);
	void SetCustomValues(string arguments);
private:
	int RestartNode(ActionType action);
};

#endif /* WATCHER_H_ */
