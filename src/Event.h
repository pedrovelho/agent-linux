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
 * Event.h
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#ifndef EVENT_H_
#define EVENT_H_
#include <string>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

namespace paagent {
/**
 * Base class for all Event classes.
 */
class Event {
public:
	Event();
	virtual ~Event();
	/**
	 * Sets the priority of the process (JVM) started by
	 * this event.
	 * @param priority the priority of the event
	 * */
	void SetProcessPriority(string priority);
	/**
	 * Sets the maximum CPU percentage available to the
	 * process (JVM) started by this event.
	 * @param max the maximum percentage of CPU used. If the
	 * max parameter is larger than 100 or less than
	 * 0 an error is logged and the value is set to 100%.
	 *
	 * */
	void SetMaxCPUUsage(int max);
	/**
	 * Gets the process priority. The available values are ?????? TODO
	 */
	string GetProcessPriority();
	/**
	 * Gets the maximum CPU percentage allowed for
	 * the processes started by this event.
	 */
	int GetMaxCPUUsage();
private:
	string process_priority;
	int max_cpu_usage;
	LoggerPtr logger;
};
} //end paagent
#endif /* EVENT_H_ */
