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

void P2PAction::SetContact(string contact) {
	this->contact = contact;
}
void P2PAction::SetProtocol(string protocol) {
	this->protocol = protocol;
}
string P2PAction::GetProtocol() const {
	return protocol;
}
string P2PAction::GetContact() const {
	return contact;
}
