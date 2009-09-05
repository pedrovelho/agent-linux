/*
 * CustomAction.cpp
 *
 *  Created on: Sep 5, 2009
 *      Author: vasile
 */

#include "CustomAction.h"

CustomAction::CustomAction() {
	// TODO Auto-generated constructor stub

}

CustomAction::~CustomAction() {
	// TODO Auto-generated destructor stub
}

void CustomAction::SetArguments(string args) {
	arguments = args;
}
string CustomAction::GetArguments() const {
	return arguments;
}
