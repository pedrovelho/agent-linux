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

#ifndef RUNNER_H
#define RUNNER_H

#include "Configuration.h"
#include <string>
#include <time.h>
#include <iostream>
#include <cc++/thread.h>
#include "ConfigParser.h"
#include "Watcher.h"
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "controller_proxy.h"

using namespace ost;

class Runner: public Thread {
public:
	Runner();
	virtual ~Runner();

	/**
	 */
	void Stop() {
	}

	/**
	 */
	void ReloadConfiguration();
	void LoadConfiguration(string xml_file);
	bool isNow(CalendarEvent *calendar);
	void setConfiguration(Configuration* new_var);
	Configuration* getConfiguration();
	void StartActions(DBus::ControllerProxy::pointer controller);
	void StopActions(DBus::ControllerProxy::pointer controller);
private:
	Configuration* configuration;
	LoggerPtr logger;
	vector<Watcher*> watchers;
	string config_file;
protected:
	virtual void run();
};

#endif // RUNNER_H
