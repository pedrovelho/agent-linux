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
 * CalendarEvent.cpp
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#include "CalendarEvent.h"
namespace paagent {
CalendarEvent::CalendarEvent() {
	// TODO Auto-generated constructor stub

}

CalendarEvent::~CalendarEvent() {
	// TODO Auto-generated destructor stub
	//	destroy(startDay);
}

void CalendarEvent::SetStartDay(std::string day) {
	startDay = day;
}

void CalendarEvent::SetStartHour(int hour) {
	startHour = hour;
}

void CalendarEvent::SetStartMinute(int minute) {
	startMinute = minute;
}

void CalendarEvent::SetStartSecond(int second) {
	startSecond = second;
}

void CalendarEvent::SetDurationDays(int days) {
	durationDays = days;
}

void CalendarEvent::SetDurationHours(int hours) {
	durationHours = hours;
}
void CalendarEvent::SetDurationMinutes(int hours) {
	durationMinutes = hours;
}
void CalendarEvent::SetDurationSeconds(int seconds) {
	durationSeconds = seconds;
}
std::string CalendarEvent::GetStartDay() {
	return startDay;
}
int CalendarEvent::GetStartHour() {
	return startHour;
}
int CalendarEvent::GetStartMinute() {
	return startMinute;
}
int CalendarEvent::GetStartSecond() {
	return startSecond;
}
int CalendarEvent::GetDurationDays() {
	return durationDays;
}
int CalendarEvent::GetDurationHours() {
	return durationHours;
}
int CalendarEvent::GetDurationMinutes() {
	return durationMinutes;
}
int CalendarEvent::GetDurationSeconds() {
	return durationSeconds;
}
long CalendarEvent::GetTotalDuration() {
	return durationSeconds + durationMinutes * 60 + durationHours * 3600
			+ durationDays * 86400;
}
} //namespace paagent
