/*
 * ConfigHandler.cpp
 *
 *  Created on: Apr 28, 2009
 *      Author: vasile
 */

#include "ConfigHandler.h"
ConfigHandler::ConfigHandler() {
	logger = log4cxx::Logger::getLogger("ConfigHandler");
	BasicConfigurator::configure();
	logger->setLevel(log4cxx::Level::getTrace());
}

ConfigHandler::~ConfigHandler() {
	// TODO Auto-generated destructor stub
}
void ConfigHandler::startDocument() {
	LOG4CXX_DEBUG(logger, "Started parsing file");
}
void ConfigHandler::endDocument() {
	LOG4CXX_DEBUG(logger, "Finished parsing file");
}
void ConfigHandler::characters(const XMLCh* const chars, unsigned int length) {
	element_data = XMLString::transcode(chars);
	LOG4CXX_TRACE(logger, "Found data:" + element_data);
}
void ConfigHandler::startElement(const XMLCh* const uri,
		const XMLCh* const localname, const XMLCh* const qname,
		const Attributes& attributes) {
	//	cout << "Found element: "
	//		 << "URI: " << XMLString::transcode(uri)
	//		 << " Local name: "<<  XMLString::transcode(localname)
	//		 << " Qualified name: " << XMLString::transcode(qname) << 	endl;
	//
	XMLSize_t len = attributes.getLength();
	//
	//	for (XMLSize_t index = 0; index < len; index++)
	//    {
	//			cout << XMLString::transcode(attributes.getURI(index))
	//			<<  XMLString::transcode(attributes.getLocalName(index))
	//				<< XMLString::transcode(attributes.getQName(index))
	//				<< XMLString::transcode(attributes.getValue(index)) << endl;
	//    }
	std::string name = XMLString::transcode(localname);
	LOG4CXX_DEBUG(logger, "Processing element [" << name << "] with [" << len << "] attributes");
	if (name == ACTION) {
		string action_name = XMLString::transcode(attributes.getValue(
				XMLString::transcode("xsi:type")));
		cout << "Action found: ";
		cout << action_name << endl;
	}

	if (name == EVENT) {
		string event_name = XMLString::transcode(attributes.getValue(
				XMLString::transcode("xsi:type")));
		cout << "Event found  " << event_name;
	}
}
void ConfigHandler::endElement(const XMLCh* const uri,
		const XMLCh* const localname, const XMLCh* const qname) {
	//	cout << "Found element: "
	//		 << "URI: " << XMLString::transcode(uri)
	//		 << " Local name: "<<  XMLString::transcode(localname)
	//		 << " Qualified name: " << XMLString::transcode(qname) << 	endl;
	//
	//
	//	for (XMLSize_t index = 0; index < len; index++)
	//    {
	//			cout << XMLString::transcode(attributes.getURI(index))
	//			<<  XMLString::transcode(attributes.getLocalName(index))
	//				<< XMLString::transcode(attributes.getQName(index))
	//				<< XMLString::transcode(attributes.getValue(index)) << endl;
	//    }
	std::string name = XMLString::transcode(localname);
	LOG4CXX_DEBUG(logger, "Found end element [" << name );
	//		if (name == ACTION) {
	//			string action_name = XMLString::transcode(attributes.getValue(
	//					XMLString::transcode("xsi:type")));
	//			cout << "Action found: ";
	//			cout << action_name << endl;
	//		}
	//
	//		if (name == EVENT) {
	//			string event_name = XMLString::transcode(attributes.getValue(
	//					XMLString::transcode("xsi:type")));
	//			cout << "Event found  " << event_name;
	//		}


	//configuration values are unique in the document, should be safe to do this
	if (name == PROACTIVE_LOCATION) {
		proactive_location = element_data;
		LOG4CXX_TRACE(logger, "Set ProActive location to " + proactive_location);
	} else if (name == JAVA_HOME) {
		java_home = element_data;
		LOG4CXX_TRACE(logger, "Set Java home location to " + java_home);
	} else if (name == JAVA_PARAMS) {
		jvm_params = element_data;
		LOG4CXX_TRACE(logger, "Set JVM parameters to " + jvm_params);
	}else if (name == JAVA_PARAMS) {
		jvm_params = element_data;
		LOG4CXX_TRACE(logger, "Set JVM parameters to " + jvm_params);
	}
}

