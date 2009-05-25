/*
 * ConfigHandler.h
 *
 *  Created on: Apr 28, 2009
 *      Author: vasile
 */

#ifndef CONFIGHANDLER_H_
#define CONFIGHANDLER_H_

#include "XMLDefs.h"
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <iostream>
using namespace xercesc;
using namespace std;

class ConfigHandler:public DefaultHandler {
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

};

#endif /* CONFIGHANDLER_H_ */
