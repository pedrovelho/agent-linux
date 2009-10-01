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
#include <typeinfo>

using namespace std;
class Event {
public:
	Event();
	virtual ~Event();
	void SetProcessPriority(string priority);
	void SetMaxCPUUsage(int max);
	string GetProcessPriority();
	int GetMaxCPUUsage();
private:
	string process_priority;
	int max_cpu_usage;
};

#endif /* EVENT_H_ */
