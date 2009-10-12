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
 * RMNodeStarter.cpp
 *
 *  Created on: Sep 16, 2009
 *      Author: vasile
 */

#include "RMNodeStarter.h"
namespace paagent {
RMNodeStarter::RMNodeStarter(string name, string java_class,
		string user, string password, string url) :
	NodeStarter(name, java_class) {
	this->user = user;
	this->password = password;
	this->url = url;

}

RMNodeStarter::~RMNodeStarter() {
	// TODO Auto-generated destructor stub
}
} //namespace paagent
