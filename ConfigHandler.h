/*
 * ConfigHandler.h
 *
 *  Created on: Apr 28, 2009
 *      Author: vasile
 */

#ifndef CONFIGHANDLER_H_
#define CONFIGHANDLER_H_

#include "XMLDefs.h"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <iostream>
#include <string>
using namespace xercesc;
using namespace std;

class ConfigHandler:public DefaultHandler {
private:
	LoggerPtr logger;
	string element_data;
	string proactive_location;
	string java_home;
	string jvm_params;
public:
	ConfigHandler();
	virtual ~ConfigHandler();
	virtual void startDocument();
	virtual void endDocument();
    virtual void startElement
    (
        const   XMLCh* const    uri,
        const   XMLCh* const    localname,
        const   XMLCh* const    qname
        , const Attributes&	attrs
    );
    virtual void characters(const   XMLCh* const chars,
    		unsigned int length);

    virtual void endElement
	(
		const XMLCh* const uri,
		const XMLCh* const localname,
		const XMLCh* const qname
	);
};

#endif /* CONFIGHANDLER_H_ */
