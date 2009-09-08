/*
 * CalendarEvent.cpp
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#include "CalendarEvent.h"
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
int CalendarEvent::GetTotalDuration() {
	return durationSeconds + durationMinutes * 60 + durationHours * 3600
			+ durationDays * 86400;
}
string CalendarEvent::GetType() {
	return typeid(*this).name();
		}
