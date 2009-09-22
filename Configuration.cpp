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
 * Configuration.cpp
 *
 *  Created on: Sep 4, 2009
 *      Author: vasile
 */

#include "Configuration.h"
#include <iostream>
Configuration* Configuration::instance = NULL;

Configuration* Configuration::Inst() {
	if (instance == NULL) {
		instance = new Configuration();
	}
	return instance;
}

//private constructor
Configuration::Configuration() {
	Initialize();
}

Configuration::~Configuration() {
	if (instance != 0)
		delete instance;
}
void Configuration::Initialize() {
}
//FIXME dekludgeme introspect much ?
void Configuration::AddAction(Action *action) {

	string action_type = typeid(*action).name();
	if (action_type == typeid(RMAction).name()) {
		rm_actions.push_back(static_cast<RMAction*> (action));
	} else if (action_type == typeid(P2PAction).name()) {
		p2p_actions.push_back(static_cast<P2PAction*> (action));
	} else if (action_type == typeid(CustomAction).name()) {
		custom_actions.push_back(static_cast<CustomAction*> (action));
	} else if (action_type == typeid(AdvertAction).name()) {
		advert_actions.push_back(static_cast<AdvertAction*> (action));
	}
}
void Configuration::AddEvent(Event *event) {
	string event_type = typeid(*event).name();
	if (event_type == typeid(CalendarEvent).name()) {
		calendar_events.push_back(static_cast<CalendarEvent*> (event));
	} else if (event_type == typeid(IdlenessEvent).name()) {
		idle_events.push_back(static_cast<IdlenessEvent*> (event));
	}
}
int Configuration::GetNoCPUS() {
	return sysconf(_SC_NPROCESSORS_ONLN);
}
string Configuration::GetClasspath() {

	string classpath;
	classpath = proactive_location + PROACTIVE_JAR + ":" + proactive_location
			+ PROACTIVE_EXAMPLES_JAR + ":" + proactive_location
			+ PROACTIVE_UTILS_JAR;
	return classpath;
}

//static int get_ncpu() {
//	int ncpu = -1;
//#ifdef _SC_NPROCESSORS_ONLN
//	ncpu = sysconf(_SC_NPROCESSORS_ONLN);
//#elif defined __APPLE__
//	int mib[2] = {CTL_HW, HW_NCPU};
//	size_t len = sizeof(ncpu);
//	sysctl(mib, 2, &ncpu, &len, NULL, 0);
//#endif
//	return ncpu;
//}
