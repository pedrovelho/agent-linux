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
 * CustomNodeStarter.cpp
 *
 *  Created on: Sep 18, 2009
 *      Author: vasile
 */

#include "CustomNodeStarter.h"
namespace paagent {
CustomNodeStarter::CustomNodeStarter(string name, string java_class,
		string arguments) :
	NodeStarter(name, java_class) {
	this->arguments = arguments;
	//add the parsed arguments to the end of the
	//vector of exec arguments
	istringstream iss(arguments);
	copy(istream_iterator<string> (iss), istream_iterator<string> (),
			back_inserter<vector<string> > (this->exec_arguments));

}

CustomNodeStarter::~CustomNodeStarter() {
	// TODO Auto-generated destructor stub
}
} //namespace paagent
