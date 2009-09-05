/*
 * RMAction.cpp
 *
 *  Created on: Sep 5, 2009
 *      Author: vasile
 */

#include "RMAction.h"

RMAction::RMAction() {
	// TODO Auto-generated constructor stub

}

RMAction::~RMAction() {
	// TODO Auto-generated destructor stub
}

void RMAction::SetUsername(string user){
	username = user;
}
void RMAction::SetPassword(string pass) {
	password = pass;
}
void RMAction::SetURL(string url){
	this->url = url;
}
string RMAction::GetPassword() const {
	return password;
}
string RMAction::GetURL() const {
	return url;
}
string RMAction::GetUsername() const {
	return username;
}
