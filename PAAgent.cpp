/*
 * PAAgent.cpp
 *
 *  Created on: Sep 1, 2009
 *      Author: vasile
 */

#include "PAAgent.h"
#include "controller_adapter.h"
PAAgent::PAAgent() {
	// TODO Auto-generated constructor stub

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
	std::cout << "Requesting name" << endl;
	// request a name on the bus
	ret = conn->request_name("proactive.agent.controller",
			DBUS_NAME_FLAG_REPLACE_EXISTING);
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		cout << "Add proper error message" << endl;
	}
	std::cout << "Declaring adapter" << endl;

	Controller controller;
	DBus::ControllerAdapter::pointer adapter = DBus::ControllerAdapter::create(
			&controller);
	std::cout << "Created adapter" << endl;
	std::cout << "Register adapter" << endl;
	conn->register_object(adapter);
	std::cout << "Running" << std::flush;

	for (;;) {
		std::cout << "." << std::flush;
		usleep(1000* 1000 );
	}

	std::cout << std::endl;
}
