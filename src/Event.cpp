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
 * Event.cpp
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#include "Event.h"
namespace paagent {

Event::Event() {
	logger = log4cxx::Logger::getLogger("Event");
	max_cpu_usage = 100;
}

Event::~Event() {
	// TODO Auto-generated destructor stub
}

void Event::SetProcessPriority(string priority) {
	process_priority = priority;
}
void Event::SetMaxCPUUsage(int max) {
	if (max < 100 && max > 0) {
		max_cpu_usage = max;
	} else {
		LOG4CXX_ERROR(logger, "The CPU percentage is out of bounds, setting to 100%."
				<< "The value passed was " << max);
		max_cpu_usage = 100;
	}
}
string Event::GetProcessPriority() {
	return process_priority;
}
int Event::GetMaxCPUUsage() {
	return max_cpu_usage;
}

} //end paagent
