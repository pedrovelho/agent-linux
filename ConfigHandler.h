/*
 * ConfigHandler.h
 *
 *  Created on: Apr 28, 2009
 *      Author: vasile
 */

#ifndef CONFIGHANDLER_H_
#define CONFIGHANDLER_H_

#include "XMLDefs.h"
using namespace xmltags;
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include "Configuration.h"
using namespace xercesc;
using namespace std;
class ConfigHandler:public DefaultHandler {
private:
	LoggerPtr logger;
	string element_data;
	Configuration *config;
	Event *currentEvent;
	Action *currentAction;
	bool processing_event;
	bool processing_action;
	bool processing_config;
	//some helper functions TODO move to helper class ?
	int stoint(string value);
	bool stobool(string value);
	void processEvent(string name, string value);
	void processAction(string name, string value);

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
