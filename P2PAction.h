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
 * P2PAction.h
 *
 *  Created on: Sep 5, 2009
 *      Author: vasile
 */

#ifndef P2PACTION_H_
#define P2PACTION_H_

#include "Action.h"

class P2PAction: public Action {
public:
	P2PAction();
	virtual ~P2PAction();

	void SetContact(string contact);
	void SetProtocol(string protocol);
	string GetContact() const;
	string GetProtocol() const;
private:
	string contact;
	string protocol;
};

#endif /* P2PACTION_H_ */
