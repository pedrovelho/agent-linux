/*
 * Event.h
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#ifndef EVENT_H_
#define EVENT_H_

class Event {
public:
	Event();
	virtual ~Event();
	void operator()();
};

#endif /* EVENT_H_ */
