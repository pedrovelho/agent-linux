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
 * CustomNodeStarter.h
 *
 *  Created on: Sep 18, 2009
 *      Author: vasile
 */

#ifndef CUSTOMNODESTARTER_H_
#define CUSTOMNODESTARTER_H_

#include "NodeStarter.h"

class CustomNodeStarter: public NodeStarter {
public:
	CustomNodeStarter(string name, string java_class, string security_policy,
			string log4j_file, string proactive_home, string classpath,
			string java_bin, string arguments);
	virtual ~CustomNodeStarter();
	int RunCommand();
private:
	string arguments;
};

#endif /* CUSTOMNODESTARTER_H_ */
