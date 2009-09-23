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

#include "Runner.h"
#include <iostream>
#include <string>
#include <tclap/CmdLine.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/file.h>

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

/**
 * Entry point for the ProActive Linux Agent command line client.
 * The main function parses the command line arguments using the
 * tclap library and search for a configuration file for the agent
 * and a configuration file for logging.
 *
 * USAGE:
 *
 * ./paagent  [-l <string>] -c <string> [--] [--version] [-h]
 *
 *
 * Where:
 *
 * -l <string>,  --logging <string>
 * Log4CXX configuration file
 *
 * -c <string>,  --configuration <string>
 * (required)  Configuration file
 *
 * --,  --ignore_rest
 * Ignores the rest of the labeled arguments following this flag.
 *
 * --version
 * Displays version information and exits.
 *
 * -h,  --help
 * Displays usage information and exits.
 *
 *
 * Starts the ProActive Linux Agent. The Agent will read an XML
 * configuration file and start and stop nodes according to an event
 * schedule or to idle thresholds.
 *
 * @param arc the name of the program
 * @param argv the arguments passed to the program, configuration and logging file
 * @see paagent_server.cpp
 * @author vjuresch
 * @since 0.1
 */
int main(int argc, char** argv) {

	try {
		//define description, delimiter, and version
		TCLAP::CmdLine cmd("Starts the ProActive Linux Agent. The Agent will "
			"read an XML configuration file "
			"and start and stop nodes according to an event "
			"schedule or to idle thresholds.", ' ', "0.1");

		//define configuration argument
		TCLAP::ValueArg<std::string> xmlFileArg("c", "configuration",
				"Configuration file", true, "", "string");
		//define logging argument
		TCLAP::ValueArg<std::string> log4cxxFileArg("l", "logging",
				"Log4CXX configuration file", false, "", "string");

		//add to command line object
		cmd.add(xmlFileArg);
		cmd.add(log4cxxFileArg);
		// Parse the argv array.
		cmd.parse(argc, argv);

		//get the values
		string xml_file = xmlFileArg.getValue();
		string log4cxx_file = log4cxxFileArg.getValue();

		//arguments logic
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

		//start agent client
		Runner runner(xml_file);
		LOG4CXX_DEBUG(logger, "Starting agent main loop...");
		runner.start();
		runner.join();

	} catch (TCLAP::ArgException &e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId()
				<< std::endl;
	}
}

