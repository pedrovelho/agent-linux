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
 * ConfigParser.cpp
 *
 *  Created on: Apr 27, 2009
 *      Author: vasile
 */

#include "ConfigParser.h"
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <iostream>
using namespace xercesc;
using namespace std;

ConfigParser::ConfigParser(std::string xmlFile) {
	logger = log4cxx::Logger::getLogger("ConfigParser");
	this->xmlFile = xmlFile;
	try {
		XMLPlatformUtils::Initialize();
	} catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		LOG4CXX_ERROR(logger, "Initialization error" << message);
		XMLString::release(&message);
	}

	parser = XMLReaderFactory::createXMLReader();
	parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true); // optional

	defaultHandler = new ConfigHandler();
	parser->setContentHandler(defaultHandler);
	parser->setErrorHandler(defaultHandler);

	LOG4CXX_INFO(logger, "Configuration parser initialized.");
}
Configuration* ConfigParser::GetConfiguration() {
	try {
		LOG4CXX_INFO(logger, "Started parsing of: " << xmlFile);
		parser->parse(xmlFile.c_str());
	} catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		LOG4CXX_ERROR(logger, "XMLException Exception message is: " << message);
		XMLString::release(&message);
		//	        return -1;
	} catch (const SAXParseException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		LOG4CXX_ERROR(logger,"SAXParseException Exception message is: " << message );
		XMLString::release(&message);
		//	        return -1;
	} catch (...) {
		LOG4CXX_ERROR(logger, "Unexpected Exception");
		//	        return -1;
	}
	return defaultHandler->GetConfiguration();

}
ConfigParser::~ConfigParser() {
	delete parser;
	delete defaultHandler;

}
