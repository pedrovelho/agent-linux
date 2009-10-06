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
 * CalendarEvent.h
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#ifndef CALENDAREVENT_H_
#define CALENDAREVENT_H_

#include "Event.h"
#include <string>
using namespace std;
/**
 * Class used for storing CalendarEvent data.
 */
class CalendarEvent: public Event {
public:
	CalendarEvent();
	virtual ~CalendarEvent();
	//setters
	void SetStartDay(std::string day);
	void SetStartHour(int hour);
	void SetStartMinute(int minute);
	void SetStartSecond(int second);
	void SetDurationDays(int days);
	void SetDurationHours(int hours);
	void SetDurationMinutes(int minutes);
	void SetDurationSeconds(int seconds);
	//getters
	string GetStartDay();
	int GetStartHour();
	int GetStartMinute();
	int GetStartSecond();
	int GetDurationDays();
	int GetDurationHours();
	int GetDurationMinutes();
	int GetDurationSeconds();
	/**
	 * Returns the total duration of
	 * the event in seconds.
	 */
	long GetTotalDuration();

private:
	string startDay;

	int startHour;
	int startMinute;
	int startSecond;

	int durationDays;
	int durationHours;
	int durationMinutes;
	int durationSeconds;

};

#endif /* CALENDAREVENT_H_ */
