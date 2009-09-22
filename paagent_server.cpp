/*
 * paagent_server.cpp
 *
 *  Created on: Sep 4, 2009
 *      Author: vasile
 */

#include <iostream>

#include <string>
#include "PAAgent.h"
#include <tclap/CmdLine.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/file.h>
#include <log4cxx/basicconfigurator.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

using namespace std;
int main(int argc, char** argv) {
	try {

		//description and version
		TCLAP::CmdLine cmd(
				"Starts the ProActive Linux Agent Server. The server will "
					"start and stop nodes and return statistics"
					" according the what methods are called through DBus ",
				' ', "0.1");

		//logging file option
		TCLAP::ValueArg<std::string> log4cxxFileArg("l", "logging",
				"Log4CXX configuration file", false, "homer", "string");

		cmd.add(log4cxxFileArg);
		// Parse the argv array.
		cmd.parse(argc, argv);

		// Get the value parsed by each arg.
		string log4cxx_file = log4cxxFileArg.getValue();

		// Arguments logic

		LoggerPtr logger = log4cxx::Logger::getRootLogger();
		//if log4cxx_file is set try to load, else do a basic config
		if (log4cxxFileArg.isSet()) {
			PropertyConfigurator::configure(log4cxx_file);
			LOG4CXX_INFO(logger, "Logging initialized from file: " << log4cxx_file);

		} else {
			BasicConfigurator::configure();
			//setting level to info since not specifying a logging file
			//means the user is only interested only in basic information
			logger->setLevel(log4cxx::Level::getInfo());
			LOG4CXX_INFO(logger, "Logging initialized with basic configuration.");
		}

		LOG4CXX_DEBUG(logger, "Starting server main loop...");

		PAAgent server;
		server.start();
		server.join();

	} catch (TCLAP::ArgException &e) // catch any exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId()
				<< std::endl;
	}
}

