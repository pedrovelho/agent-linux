/*
 * P2PAction.cpp
 *
 *  Created on: Sep 5, 2009
 *      Author: vasile
 */

#include "P2PAction.h"

P2PAction::P2PAction() {
	// TODO Auto-generated constructor stub

}

P2PAction::~P2PAction() {
	// TODO Auto-generated destructor stub
}

void P2PAction::SetContact(string contact){
	this->contact = contact;
}
void P2PAction::SetProtocol(string protocol){
	this->protocol = protocol;
}
