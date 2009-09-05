/*
 * Event.cpp
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#include "Event.h"

Event::Event() {
	// TODO Auto-generated constructor stub

}

Event::~Event() {
	// TODO Auto-generated destructor stub
}

void Event::SetProcessPriority(string priority){
	process_priority = priority;
}
void Event::SetMaxCPUUsage(int max){
	max_cpu_usage = max;
}
string Event::GetProcessPriority(){
	return process_priority;
}
int Event::GetMaxCPUUsage(){
	return max_cpu_usage;
}
