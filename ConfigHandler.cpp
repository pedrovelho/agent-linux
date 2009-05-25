/*
 * ConfigHandler.cpp
 *
 *  Created on: Apr 28, 2009
 *      Author: vasile
 */

#include "ConfigHandler.h"
ConfigHandler::ConfigHandler() {
	// TODO Auto-generated constructor stub

}

ConfigHandler::~ConfigHandler() {
	// TODO Auto-generated destructor stub
}
void ConfigHandler::startDocument(){
	cout << "Started document ...\n";
}
void ConfigHandler::endDocument(){
	cout << "End of document...\n";
}

void ConfigHandler::startElement
(
    const   XMLCh* const    uri,
    const   XMLCh* const    localname,
    const   XMLCh* const    qname,
    const Attributes&	attributes
){
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
	//meh
	if (name == ACTION){
		string action_name = XMLString::transcode(attributes.getValue(XMLString::transcode("xsi:type")));
		cout << "Action found: ";
		cout <<	action_name << endl;
	}



	if (name == EVENT){
		cout << "Event found \n";
	}
}
