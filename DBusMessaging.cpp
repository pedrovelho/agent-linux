/*
 * DBusMessaging.cpp
 *
 *  Created on: Aug 28, 2009
 *      Author: vasile
 */

#include "DBusMessaging.h"

DBusMessaging* DBusMessaging::instance = NULL;

DBusMessaging* DBusMessaging::Inst(string connection_name) {
	if (instance == NULL) {
		instance = new DBusMessaging(connection_name);
	}
	return instance;
}

DBusMessaging* DBusMessaging::Inst() {
	if (instance == NULL) {
		instance = new DBusMessaging(DEFAULT_CONNECTION_NAME);
	}
	return instance;
}

//private constructor
DBusMessaging::DBusMessaging(string connection_name) {
	//initialize logger
	//TODO  move outside the constructor
	logger = log4cxx::Logger::getLogger("DBus Messaging singleton ");
	//	logger->setAdditivity(false);


	//	ConsoleAppender *appender = new ConsoleAppender();

	//	logger->removeAllAppenders();
	//	appender->setTarget("System.out");
	//	appender->setWriter()
	//	logger->addAppender(appender);
	logger->setLevel(log4cxx::Level::getDebug());
	this->connection_name = connection_name;
	Initialize();
}

DBusMessaging::~DBusMessaging() {
	if (instance != 0)
		delete instance;
}
void DBusMessaging::Initialize() {

	DBusError err;
	int ret;

	LOG4CXX_DEBUG(logger, "Connecting to DBUS...");

	// initialize the error
	dbus_error_init(&err);

	// connect to the bus and check for errors
	connection = dbus_bus_get(DBUS_BUS_SESSION, &err);
	if (dbus_error_is_set(&err)) {
		LOG4CXX_ERROR(logger, "Connection Error " << err.message);
		dbus_error_free(&err);
	}
	if (NULL == connection) {
		LOG4CXX_ERROR(logger, "Connection Null");
	}

	// request our name on the bus and check for errors
	ret = dbus_bus_request_name(connection, connection_name.c_str(),
			DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
	if (dbus_error_is_set(&err)) {
		LOG4CXX_ERROR(logger, "Name Error:  " << err.message);
		dbus_error_free(&err);
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		LOG4CXX_ERROR(logger, "Not Primary Owner " + ret);
	}
	LOG4CXX_DEBUG(logger, "Connected with name to D-BUS with "
			<< connection_name);
}

DBusConnection* DBusMessaging::GetConnection() {
	return connection;
}

void DBusMessaging::SendSignal(string signal, string node_name) {
	DBusMessage* msg;
	DBusMessageIter args;
	DBusError err;
	dbus_uint32_t serial = 234324;

	// initialise the error value
	dbus_error_init(&err);

	LOG4CXX_DEBUG(logger, "Sending [" << signal << "]");

	// create a signal & check for errors
	msg = dbus_message_new_signal("/proactive/watcher", // object name of the signal
			"proactive.controller", // interface name of the signal
			signal.c_str()); // name of the signal
	LOG4CXX_TRACE(logger, "Message created");
	if (NULL == msg) {
		LOG4CXX_ERROR(logger, "The message is NULL");
	}
	//FIXME - blocks here for some reason
	//works without, but extra info cannot be sent (args)
	//
	//	// append arguments onto signalTIMEOUT
	//	dbus_message_iter_init_append(msg, &args);
	////	dbus_message_iter_init(msg, &args);
	//	cout << "Appending arguments..." << endl;
	//
	//	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING,
	//			message.c_str())) {
	//		LOG4CXX_ERROR(logger, "Out of memory");
	//	}
	//	cout << "Arguments appended" << endl;

	// send the message and flush the connection
	if (!dbus_connection_send(connection, msg, &serial)) {
		LOG4CXX_ERROR(logger, "Out of memory");
	}
	LOG4CXX_TRACE(logger, "Signal sent through connection");

	dbus_connection_flush(connection);
	LOG4CXX_TRACE(logger, "Connection flushed");

	// free the message
	dbus_message_unref(msg);
}
