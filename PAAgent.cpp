/*
 * PAAgent.cpp
 *
 *  Created on: Sep 1, 2009
 *      Author: vasile
 */

#include "PAAgent.h"
#include "controller_adapter.h"
PAAgent::PAAgent() {
	logger = log4cxx::Logger::getLogger("PAAgent");
}

PAAgent::~PAAgent() {
	// TODO Auto-generated destructor stub
}

void PAAgent::run() {
	DBus::init();

	int ret;

	DBus::Dispatcher dispatcher;

	DBus::Connection::pointer conn = dispatcher.create_connection(
			DBus::BUS_SESSION);
	LOG4CXX_DEBUG(logger, "Requesting name on DBus");
	// request a name on the bus
	ret = conn->request_name("proactive.agent.controller",
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
