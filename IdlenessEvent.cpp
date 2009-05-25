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

void IdlenessEvent::SetBeginSecs(int secs){
	beginSecs = secs;
}
void IdlenessEvent::SetEndSecs(int secs){
	endSecs = secs;
}

void IdlenessEvent::SetBeginThreshold(int threshold){
	beginThreshold = threshold;
}

void IdlenessEvent::SetEndThreshold(int threshold){
	endThreshold = threshold;
}
