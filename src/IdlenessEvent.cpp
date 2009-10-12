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
 * IdlenessEvent.cpp
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#include "IdlenessEvent.h"
namespace paagent {
IdlenessEvent::IdlenessEvent() {
	// TODO Auto-generated constructor stub

}

IdlenessEvent::~IdlenessEvent() {
	// TODO Auto-generated destructor stub
}

void IdlenessEvent::SetBeginSeconds(int secs) {
	beginSecs = secs;
}
void IdlenessEvent::SetEndSeconds(int secs) {
	endSecs = secs;
}

void IdlenessEvent::SetBeginThreshold(int threshold) {
	beginThreshold = threshold;
}

void IdlenessEvent::SetEndThreshold(int threshold) {
	endThreshold = threshold;
}
int IdlenessEvent::GetBeginSeconds() const {
	return beginSecs;
}
int IdlenessEvent::GetEndSeconds() const {
	return endSecs;
}
int IdlenessEvent::GetBeginThreshold() const {
	return beginThreshold;
}
int IdlenessEvent::GetEndThreshold() const {
	return endThreshold;
}
} //namespace paagent
