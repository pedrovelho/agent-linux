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
		this->xmlFile = xmlFile;
	    try {
	        XMLPlatformUtils::Initialize();
	    }
	    catch (const XMLException& toCatch) {
	        char* message = XMLString::transcode(toCatch.getMessage());
	        cout << "Error during initialization! :\n";
	        cout << "Exception message is: \n"
	             << message << "\n";
	        XMLString::release(&message);
	    }

	    parser = XMLReaderFactory::createXMLReader();
	    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);   // optional

	    defaultHandler = new ConfigHandler();
	    parser->setContentHandler(defaultHandler);
	    parser->setErrorHandler(defaultHandler);
	    cout << "Configuration parser initialized...\n";
}
void ConfigParser::startNodes(){
    try {
    	cout << "Started parsing of: " << xmlFile << endl;
        parser->parse(xmlFile.c_str());
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "XMLException Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
//	        return -1;
    }
    catch (const SAXParseException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "SAXParseException Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
//	        return -1;
    }
    catch (...) {
        cout << "Unexpected Exception \n" ;
//	        return -1;
    }


}
ConfigParser::~ConfigParser() {
    delete parser;
    delete defaultHandler;

}



//
//using namespace std;
//using namespace xercesc;
//
//int main (int argc, char* args[]) {
//
//    try {
//        XMLPlatformUtils::Initialize();
//    }
//    catch (const XMLException& toCatch) {
//        char* message = XMLString::transcode(toCatch.getMessage());
//        cout << "Error during initialization! :\n";
//        cout << "Exception message is: \n"
//             << message << "\n";
//        XMLString::release(&message);
//        return 1;
//    }
//
//    char* xmlFile = "x1.xml";
//    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
//    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
//    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);   // optional
//
//    DefaultHandler* defaultHandler = new DefaultHandler();
//    parser->setContentHandler(defaultHandler);
//    parser->setErrorHandler(defaultHandler);
//
//    try {
//        parser->parse(xmlFile);
//    }
//    catch (const XMLException& toCatch) {
//        char* message = XMLString::transcode(toCatch.getMessage());
//        cout << "Exception message is: \n"
//             << message << "\n";
//        XMLString::release(&message);
//        return -1;
//    }
//    catch (const SAXParseException& toCatch) {
//        char* message = XMLString::transcode(toCatch.getMessage());
//        cout << "Exception message is: \n"
//             << message << "\n";
//        XMLString::release(&message);
//        return -1;
//    }
//    catch (...) {
//        cout << "Unexpected Exception \n" ;
//        return -1;
//    }
//
//    delete parser;
//    delete defaultHandler;
//    return 0;
//}
