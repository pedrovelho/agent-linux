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
