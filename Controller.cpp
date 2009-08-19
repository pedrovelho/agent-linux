/*
 * Controller.cpp
 *
 *  Created on: Mar 19, 2009
 *      Author: vasile
 */

#include "NodeStarter.h"
#include "Controller.h"
#include "Constants.h"
// include log4cxx header files.
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"
#include <pthread.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

Controller::Controller() {
	//initialize logger
	//TODO  move outside the constructor
	logger = log4cxx::Logger::getLogger("Controller");
	BasicConfigurator::configure();
	logger->setLevel(log4cxx::Level::getDebug());

	//connect to dbus
	ListenSignals(dbus_connection);
}

Controller::~Controller() {
	//disconnect from dbus
}

int Controller::StartNode(string shell, string node_executable,
		string node_name) {


	NodeStarter starter(shell, node_executable,node_name);
//	starter.start();
	starter.run();

//	LOG4CXX_DEBUG(logger, "Node started");
	return starter.getPid();

}

int Controller::StartNode(string node_name) {
	return StartNode(default_shell, default_node_exec, node_name);
}

void Controller::SetTick(int pid, int tick) {
	LOG4CXX_TRACE(logger, "Changing tick to " << tick << " for pid " << pid );
	pids_ticks.insert(pair<int, int> (pid, tick));
	LOG4CXX_TRACE(logger, "The size of the maps containing the pids and ticks is now " << pids_ticks.size());
}

void Controller::SetWatcherPid(int process_pid, int watcher_pid) {
	//	using namespace boost::interprocess;
	//	   try{
	//		   //attach memory segment
	//	      SharedMap::iterator it;
	//		   managed_shared_memory segment
	//	         (open_only
	//	         ,SHARED_SEGMENT_NAME);  //segment name
	//	      //Find the vector using the c-string name
	//	      SharedMap *pids_watchers = segment.find<SharedMap>(PIDS_WATCHERS).first;
	//	      //first_process, old_watcher
	//	      SharedMap *first_process_watcher = segment.find<SharedMap>(PIDS_WATCHERS).first;
	//
	//
	//
	//	      //old_process, old_watcher
	//	      //get old_watcher
	//	      it = pids_watchers->find(process_pid);
	//
	//	      //first_process, old_watcher
	//	      //get first_process by old_watcher
	//	   //   it = first_process_watcher->find((*it).second);
	//
	//	      //erase old pair
	//	      cout << "First process pid:  "<<(*it).first << endl;
	//
	//	      cout << "Old Watcher pid:  "<<(*it).second <<endl;
	//	      first_process_watcher->erase((*it).second);
	//
	//	      //first_process, new_watcher
	//	      //set new_watcher
	//	      first_process_watcher->insert(std::pair<const int, int>((*it).first, watcher_pid));
	//
	//
	//	      //delete old value by key
	//	      pids_watchers->erase(process_pid);
	//	      //Insert data in the map
	//	      pids_watchers->insert(std::pair<const int, int>(process_pid, watcher_pid));
	//
	//	   }
	//	   catch(...){
	//		   cout << "Catched";
	//	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	//	      throw;
	//	   }
	LOG4CXX_ERROR(logger, "SetWatchet pid method not implemented ");

}

int Controller::GetWatcherPid(int process_pid) {
	//
	//	using namespace boost::interprocess;
	//	   try{
	//		   SharedMap::iterator it;
	//		   //attach memory segment
	//		   managed_shared_memory segment
	//	         (open_only
	//	         ,SHARED_SEGMENT_NAME);  //segment name
	//	      //Find the vector using the c-string name
	//	      SharedMap *pids_watchers = segment.find<SharedMap>(PIDS_WATCHERS).first;
	//	      //returning the tick;
	//		it = pids_watchers->find(process_pid);
	//	    return (*it).second;
	//	   }
	//	   catch(...){
	//		   cout << "Catched";
	//	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	//	      throw;
	//	   }
	LOG4CXX_ERROR(logger, "Method not implemented");
	return false;

}

int Controller::GetTick(int pid) {
	map<int, int>::iterator iter;
	iter = pids_ticks.find(pid);
	int tick = 0;
	if (iter != pids_ticks.end()) {
		tick = iter->second;
		LOG4CXX_TRACE(logger, "Tick value retrieved for pid " << pid << " is " << tick );
	} else {
		LOG4CXX_ERROR(logger, "No tick value found for pid " << pid);
	}
	return tick;
}

bool Controller::StopWatcher(int pid) {
	//	//send a kill signal to the watcher process
	//	 using namespace boost::interprocess;
	//	 try{
	//		   SharedMap::iterator it;
	//		  //attach shared memory segment
	//	      managed_shared_memory segment
	//	         (open_only
	//	         ,SHARED_SEGMENT_NAME);  //segment name
	//	      //Find the vector using the c-string name
	//	      SharedMap *pids_watchers = segment.find<SharedMap>(PIDS_WATCHERS).first;
	//	      SharedMap *pid_ticks = segment.find<SharedMap>(PIDS_TICKS).first;
	//
	//	      //returning the tick;
	//		it = pids_watchers->find(pid);
	//	    int watcher_pid = (*it).second;
	//	    //terminate the watcher process if the watcher pid is found
	//	    if (it != pids_watchers->end()) {
	//	    	kill(watcher_pid, 15);
	//	    	//remove references from shared memory
	//	    	pids_watchers->erase(pid);
	//	    	pid_ticks->erase(pid);
	//	    	return true;
	//	    }
	//	    else{
	//	    	//return false if the pid was not found
	//	    	return false;
	//	    }
	//	   }
	//	   catch(...){
	//	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	//	      throw;
	//	   }
	//	  LOG4CXX_ERROR(logger, "Error"<< endl;
	LOG4CXX_ERROR(logger, "Method not implemented ");
	return false;
}

bool Controller::StopWatcher(string node_name) {
	//	std::map<std::string,int>::iterator it;
	//	// return the first pid of the Node (see explanation in map declaration)
	//
	//		it = nodes_pids.find(node_name);
	//	    //if a pid is found get the current pid for the watcher and terminate
	//	    if (it != nodes_pids.end()) {
	//	    	int first_pid = (*it).second;
	//	    	cout << "Found first pid associated to Node name:" << first_pid <<endl;
	//	   	 using namespace boost::interprocess;
	//	   	 try{
	//	   		   SharedMap::iterator current_pid_it;
	//	   	      managed_shared_memory segment
	//	   	         (open_only
	//	   	         ,SHARED_SEGMENT_NAME);  //segment name
	//	   	      //Find the vector using the c-string name
	//	   	      SharedMap *old_new_pids = segment.find<SharedMap>(FIRST_PROCESS_WATCHER_PID).first;
	//	   	      //returning the tick;
	//	   	   current_pid_it = old_new_pids->find(first_pid);
	//	   	    int watcher_pid = (*current_pid_it).second;
	//		   	cout << "Found current_pid associated to Node name:" << watcher_pid <<endl;
	//
	//	   	 return StopWatcher(watcher_pid);
	//	   	   }
	//	   	   catch(...){
	//	   	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	//	   	      throw;
	//	   	   }
	//	    }
	//	    else{
	//	    	//return false if the node_name was not found
	//	    	return false;
	//	    }
	LOG4CXX_ERROR(logger, "Method not implemented ");
	return false;

}

void Controller::ResetTicks() {

}

void Controller::SetDefaultShell(string shell) {
	default_shell = shell;
}
string Controller::GetDefaultShell() {
	return default_shell;
}
void Controller::SetDefaultNodeStarter(string node_executable) {
	default_node_exec = node_executable;
}
string Controller::GetDefaultNodeStarter() {
	return default_node_exec;
}

void Controller::AssociateNodesPids(int pid, std::string node) {
	nodes_pids.insert(std::pair<std::string, int>(node, pid));
}
LoggerPtr Controller::getLogger() {
	return logger;
}
/************************************************************************************
 *******************             DBUS RELATED CODE                 ******************
 ************************************************************************************/
DBusConnection* Controller::ConnectToDBUS(string name) {

	DBusConnection* connection;
	DBusError err;
	int ret;

	LOG4CXX_DEBUG(logger, "Connecting to DBUS..." );

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
	ret = dbus_bus_request_name(connection, name.c_str(),
			DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
	if (dbus_error_is_set(&err)) {
		LOG4CXX_ERROR(logger, "Name Error:  " << err.message );
		dbus_error_free(&err);
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		LOG4CXX_ERROR(logger, "Not Primary Owner " + ret );
	}
	LOG4CXX_DEBUG(logger, "Connected with name to D-BUS with " << name );
	return connection;
}

/**
 * Listens for signals on the bus in a new thread
 */
void Controller::ListenSignals(DBusConnection *dbus_connection) {

	pid_t pid, sid;

	/* already a daemon */
	//	if ( getppid() == 1 ) return 0;

	/* Fork off the parent process */
	pid = fork();
	//If the fork was successful execute child code
	if (pid == 0) {
		/* At this point we are executing as the child process */
		/* Change the file mode mask */
		umask(0);
		/* Create a new SID for the child process */
		sid = setsid();
		LOG4CXX_DEBUG(logger, "Forked for listening signals with sid " << sid);
		//get the shotgun ready for zombies ?
		//
		//		if (sid < 0) {
		//			exit(EXIT_FAILURE);
		//		}

		/* Change the current working directory.  This prevents the current
		 directory from being locked; hence not being able to remove it. */
		chdir("/");
		/* Redirect standard files to /dev/null */
		//		freopen( "/dev/null", "r", stdin);
		//		freopen( "/dev/null", "w", stdout);
		//		freopen( "/dev/null", "w", stderr);
		//otherwise return the pid (=0)

		DBusMessage* msg;
		DBusMessageIter args;
		DBusConnection * dbus_connection;
		DBusError err;
		int ret;
		char* sigvalue;

		//connect to d-bus
		dbus_connection = ConnectToDBUS(PROACTIVE_DBUS_SERVER);

		// initialise the errors
		dbus_error_init(&err);

		// add a rule for which messages we want to see
		// see signals from the given interface

		dbus_bus_add_match(dbus_connection,
				"type='signal',interface='proactive.controller'", &err);
		dbus_connection_flush(dbus_connection);
		if (dbus_error_is_set(&err)) {
			LOG4CXX_ERROR(logger, "Match Error " << err.message);
			exit(1);
		}

		LOG4CXX_TRACE(logger, "Listening rules set");
		LOG4CXX_TRACE(logger, "Listening for signals...");

		// loop listening for signals being emmitted
		while (true) {

			// non blocking read of the next available message
			dbus_connection_read_write(dbus_connection, 0);
			msg = dbus_connection_pop_message(dbus_connection);

			// loop again if we haven't read a messagesent
			if (NULL == msg) {
				LOG4CXX_TRACE(logger, "No message received, next check in " << LISTEN_SIGNAL_TIMEOUT << " seconds");
				sleep(LISTEN_SIGNAL_TIMEOUT);
				continue;
			}
			LOG4CXX_DEBUG(logger,"Received message...");
			ProcessSignal(msg);
			// free the message
			dbus_message_unref(msg);
		}
	}
	//failed to fork
	else if (pid < 0) {
		LOG4CXX_ERROR(logger, "Failed to fork in listen signals" );
	} else {
		LOG4CXX_TRACE(logger, "Returning from parent fork in listen signals");
	}
}

/**
 * Connect to the DBUS bus and send a broadcast signal
 */
void Controller::SendSignal(string signal, string node_name) {
	DBusMessage* msg;
	DBusMessageIter args;
	DBusConnection* connection;
	DBusError err;
	dbus_uint32_t serial = 234324;

	// initialise the error value
	dbus_error_init(&err);

	connection = ConnectToDBUS(PROACTIVE_DBUS_WATCHER);
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
	LOG4CXX_TRACE(logger,"Signal sent through connection" );

	dbus_connection_flush(connection);
	LOG4CXX_TRACE(logger,"Connection flushed" );

	// free the message
	dbus_message_unref(msg);
}

void Controller::ProcessSignal(DBusMessage *message) {
	//check for interface and name
	string interface = dbus_message_get_interface(message);
	string signature = dbus_message_get_signature(message);
	string path = dbus_message_get_path(message);
	//singal
	string signal = dbus_message_get_member(message);
	LOG4CXX_DEBUG(logger, "Processing message from interface [" <<
			interface << "]  signature [" << signature <<
			"] path [" << path << "] signal [" << signal << "]" );
	if (signal == JVM_STOPPED) {
		LOG4CXX_INFO(logger,"A Node has been stopped, restarting");
//		StartWatcher("node_1", StartNode("node_1"));
		return;
	}
	LOG4CXX_DEBUG(logger,"Signal does not have an action associated");
	return;

	//	if (dbus_message_is_signal(msg, "proactive.controller",
	//			"JVM_STOPPED")) {
	//
	//		LOG4CXX_DEBUG(logger, "Processing message" );
	//		// read the parameters
	//		if (!dbus_message_iter_init(msg, &args)){
	//			LOG4CXX_TRACE(logger,"Message Has No Parameters");
	//		}
	//		else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(
	//				&args)){
	//			LOG4CXX_TRACE(logger,"Argument is not string!");
	//
	//		}
	//		else
	//			dbus_message_iter_get_basic(&args, &sigvalue);
	//
	//		LOG4CXX_DEBUG(logger,"Got Signal with value " <<  sigvalue);
	//	}

}
