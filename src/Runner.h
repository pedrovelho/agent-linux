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
#include "Constants.h"
using namespace ost;
using namespace paagent::constants;
namespace paagent{
/**
 * Class containing the logic for starting and stopping actions according to  idle and events  configuration.
 * @author vjuresch
 * @since 0.1
 */
class Runner: public Thread {
public:
	/** Constructor taking as parameter a filename containing the XML configuration
	 * @param xml_file XML configuration file
	 * */
	Runner(string xml_file);
	virtual ~Runner();

	/**
	 * Reloads the configuration from the set configuration file - TODO NOT IMPLEMENTED.
	 */
	void ReloadConfiguration();
	/**
	 * Set a XML configuration file.
	 * @param xml_file XML configuration file
	 * */
	void SetConfigurationFile(string xml_file);
	/**
	 * Verifies if a {@link CalendarEvent} time is now.
	 *
	 * @param calendar a pointer to a CalendarEvent
	 * @return true if time is now false otherwise
	 */
	bool isNow(CalendarEvent *calendar);
	/**
	 * Set the configuration used by the Runner class.
	 * @param config a pointer to a {@link Configuration} class
	 */
	void setConfiguration(Configuration* config);
	/**
	 * Get the Configuration object used for this Runner instance.
	 * @return controller a pointer to a Configuration object
	 */
	Configuration* getConfiguration();
	/**
	 * Checks which actions are enabled and starts them.
	 *
	 * An event starts all the enabled actions and
	 * a stop event also stops all the actions.
	 *
	 * @param controller a pointer to a ControllerProxy object
	 */
	void StartActions(DBus::ControllerProxy::pointer controller, int limit);
	/**
	 * Stops all the started actions and watchers.
	 *
	 * @param controller a pointer to a ControllerProxy object
	 * */
	void StopActions(DBus::ControllerProxy::pointer controller);
private:
	/** A pointer to a Configuration object used by this Runner class. */
	Configuration* configuration;
	LoggerPtr logger;
	/** A vector of watchers started to keep action nodes running. */
	vector<Watcher*> watchers;
	/** XML configuration file from which the Configuration object is created. */
	string config_file;
protected:
	/**
	 * This is the main loop of the ProActive agent.
	 *
	 * 	Assuming:
	 * <ol>
	 * 		<li>calendar/idle events DO NOT OVERLAP </li>
	 * 		<li> calendar events have precedence over idle events </li>
	 * 		<li> there is only 1 event running at the time (although the code is bit more general) </li>
	 * 		<li> all events start the same actions </li>
	 * 		<li> ?time taken in loop, except for sleep is negligible </li>
	 * </ol>
	 *
	 * \image html run_in_Runner.png
	 * The method reads the configuration file using a {@link ConfigParser} object, connects to DBus using
	 * a {@link DBUS::ControllerProxy} object and starts a loop that checks if a CalendarEvent is due or idleness
	 * thresholds specified in IdlenessEvents have been reached.
	 *
	 * To check for idleness /proc/stat is being read at two points in time and the idleness is calculated
	 * using the number of CPUs in the machine.
	 *
	 * The timers in the loop are for idleness start (taken from the XML configuration file) and
	 * for CalendarEvent stop. The idleness counter is being decreased if the CPU is between thresholds and
	 * reset if it is outside  the thresholds. If there are any CalendarEvents running the stop timer is being
	 * decreased for each iteration through the loop. The timers are decreased with a sleep_time value.
	 *
	 * */
	virtual void run();
};
} //namespace paagent
#endif // RUNNER_H
