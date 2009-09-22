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
 * P2PNodeStarter.cpp
 *
 *  Created on: Sep 18, 2009
 *      Author: vasile
 */

#include "P2PNodeStarter.h"

P2PNodeStarter::P2PNodeStarter(string name, string java_class,
		string security_policy, string log4j_file, string proactive_home,
		string classpath, string java_bin, string contact) :
	NodeStarter(name, java_class, security_policy, log4j_file, proactive_home,
			classpath, java_bin) {
	this->contact = contact;
}

P2PNodeStarter::~P2PNodeStarter() {
	// TODO Auto-generated destructor stub
}
//what's contact good for ?
int P2PNodeStarter::RunCommand() {
	return execl(java_bin.c_str(), " ", DEFAULT_DSECURITY_MANAGER.c_str(),
			security_policy.c_str(), log4j_file.c_str(),
			proactive_home.c_str(), "-classpath", classpath.c_str(),
			java_class.c_str(), name.c_str(), (char *) 0);
}

