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
 * Watcher.hb
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
#include "XMLDefs.h"
using namespace xmltags;

using namespace paagent::constants;
namespace paagent {
class Watcher: public Thread {
private:
	log4cxx::LoggerPtr logger;
	int pid;
	int tick;
	bool stop;
	DBus::ControllerProxy::pointer controller;
	ActionType action_select;
	Action *action;
	Event *event;
public:

	static Watcher* AdvertWatcher(int jvm_pid, int tick, AdvertAction *action,
			Event *event, DBus::ControllerProxy::pointer controller);
	//named constructors for different types of action restarts
	//AdvertAction
	static Watcher* AdvertWatcher(int jvm_pid, int tick, int restart_delay,
			string name, string java_class,
			DBus::ControllerProxy::pointer controller);
	//RMAction
	static Watcher* RMWatcher(int jvm_pid, int tick, RMAction *action,
			Event *event, DBus::ControllerProxy::pointer controller);
	//P2PAction
	static Watcher* P2PWatcher(int jvm_pid, int tick, P2PAction *action,
			Event *event, DBus::ControllerProxy::pointer controller);
	//CustomAction
	static Watcher* CustomWatcher(int jvm_pid, int tick, CustomAction *action,
			Event *event, DBus::ControllerProxy::pointer controller);

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
	Watcher(int jvm_pid, int tick, Action *action, Event *event,
			DBus::ControllerProxy::pointer controller, ActionType action_type);

	//these should be private
	void SetRMValues(string user, string password, string url);
	void SetP2PValues(string contact);
	void SetCustomValues(string arguments);
	void limit(int limit);
	void renice(string priority);
	string GetName() const;
private:
	int RestartNode(ActionType action);
};
} //namespace paagent
#endif /* WATCHER_H_ */
