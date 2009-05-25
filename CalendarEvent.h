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
	std::string GetStartDay();
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
	int GetTotalDuration();

private:
	std::string startDay;

	int startHour;
	int startMinute;
	int startSecond;

	int durationDays;
	int durationHours;
	int durationMinutes;
	int durationSeconds;

};

#endif /* CALENDAREVENT_H_ */
