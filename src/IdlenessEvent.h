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
 * IdlenessEvent.h
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#ifndef IDLENESSEVENT_H_
#define IDLENESSEVENT_H_

#include "Event.h"
namespace paagent {
class IdlenessEvent: public Event {
public:
	IdlenessEvent();
	virtual ~IdlenessEvent();
	void SetBeginSeconds(int secs);
	void SetEndSeconds(int secs);
	void SetBeginThreshold(int threshold);
	void SetEndThreshold(int threshold);

	int GetBeginSeconds() const;
	int GetEndSeconds() const;
	int GetBeginThreshold() const;
	int GetEndThreshold() const;

private:
	int beginSecs;
	int endSecs;
	int beginThreshold;
	int endThreshold;
};
} //namespace paagent
#endif /* IDLENESSEVENT_H_ */
