/*
 * IdlenessEvent.cpp
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#include "IdlenessEvent.h"

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
