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
 * RMAction.h
 *
 *  Created on: Sep 5, 2009
 *      Author: vasile
 */

#ifndef RMACTION_H_
#define RMACTION_H_

#include "Action.h"

class RMAction: public Action {
public:
	RMAction();
	virtual ~RMAction();
	void SetUsername(string user);
	void SetPassword(string pass);
	void SetURL(string url);

	string GetUsername() const;
	string GetPassword() const;
	string GetURL() const;

private:
	string username;
	string password;
	string url;
};

#endif /* RMACTION_H_ */
