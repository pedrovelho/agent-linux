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
 * PAAgent.cpp
 *
 *  Created on: Sep 1, 2009
 *      Author: vasile
 */

#include "PAAgent.h"
#include "controller_adapter.h"
namespace paagent {
PAAgent::PAAgent() {
	logger = log4cxx::Logger::getLogger("PAAgent");
}

PAAgent::~PAAgent() {
	// TODO Auto-generated destructor stub
}

void PAAgent::run() {
	DBus::init(true);

	int ret;

	DBus::Dispatcher dispatcher;

	DBus::Connection::pointer conn = dispatcher.create_connection(
			DBus::BUS_SESSION);
	LOG4CXX_DEBUG(logger, "Requesting name on DBus");
	// request a name on the bus
	ret = conn->request_name(PROACTIVE_DBUS_NAME,
			DBUS_NAME_FLAG_REPLACE_EXISTING);
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		LOG4CXX_ERROR(logger, "Server is not primary owner");
	}
	LOG4CXX_DEBUG(logger, "Declaring adapter")
	Controller controller;
	DBus::ControllerAdapter::pointer adapter = DBus::ControllerAdapter::create(
			&controller);

	LOG4CXX_DEBUG(logger, "Registering adapter");
	conn->register_object(adapter);
	LOG4CXX_INFO(logger, "Server is running...");

	for (;;) {
		usleep(1000* 1000 );
	}
}
}//namespace paagent
