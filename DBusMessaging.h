/*
 * DBusMessaging.h
 *
 *  Created on: Aug 28, 2009
 *      Author: vasile
 */

#ifndef DBUSMESSAGING_H_
#define DBUSMESSAGING_H_

#include <stdlib.h>
#include <dbus-1.0/dbus/dbus.h>
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"

#include <string>
using namespace std;
using namespace log4cxx;
using namespace log4cxx::helpers;
const string DEFAULT_CONNECTION_NAME="pa.controller";

class DBusMessaging {
private:
	log4cxx::LoggerPtr logger;
	DBusConnection *connection;
	string connection_name;
public:
	static DBusMessaging* Inst(string connection_name);
	static DBusMessaging* Inst();
	DBusConnection* GetConnection();
	void SendSignal(string signal, string node_name);
	virtual ~DBusMessaging();
protected:
	DBusMessaging(string connection_name);
private:
	static DBusMessaging* instance;
	//responsible with constructing a D-Bus connection
	void Initialize();
};

#endif /* DBUSMESSAGING_H_ */
