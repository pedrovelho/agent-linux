/*
 * Action.cpp
 *
 *  Created on: Sep 5, 2009
 *      Author: vasile
 */

#include "Action.h"

Action::Action() {
	// TODO Auto-generated constructor stub

}

Action::~Action() {
	// TODO Auto-generated destructor stub
}

void Action::SetRestartDelay(int delay) {
	restart_delay = delay;
}

void Action::SetStarterClass(string starter) {
	starter_class = starter;
}
void Action::SetNodeName(string name) {
	node_name = name;
}

void Action::SetEnabled(bool enabled) {
	this->enabled = enabled;
}
int Action::GetRestartDelay() const {
	return restart_delay;
}
string Action::GetStarterClass() const {
	return starter_class;

}
string Action::GetNodeName() const {
	return node_name;
}
bool Action::IsEnabled() const {
	return enabled;
}

