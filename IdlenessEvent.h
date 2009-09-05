/*
 * IdlenessEvent.h
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#ifndef IDLENESSEVENT_H_
#define IDLENESSEVENT_H_

#include "Event.h"

class IdlenessEvent: public Event {
public:
	IdlenessEvent();
	virtual ~IdlenessEvent();
	void SetBeginSeconds(int secs);
	void SetEndSeconds(int secs);
	void SetBeginThreshold(int threshold);
	void SetEndThreshold(int threshold);

	int GetBeginSeconds();
	int GetEndSeconds();
	int GetBeginThreshold();
	int GetEndThreshold();

private:
	int beginSecs;
	int endSecs;
	int beginThreshold;
	int endThreshold;
};

#endif /* IDLENESSEVENT_H_ */
