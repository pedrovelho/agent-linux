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
 * ConfigHandler.h
 *
 *  Created on: Apr 28, 2009
 *      Author: vasile
 */

#ifndef CONFIGHANDLER_H_
#define CONFIGHANDLER_H_

#include "Helpers.h"
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
class ConfigHandler: public DefaultHandler {
private:
	LoggerPtr logger;
	string element_data;
	Configuration *config;
	Event *currentEvent;
	Action *currentAction;
	bool processing_event;
	bool processing_action;
	bool processing_config;
	void processEvent(string name, string value);
	void processAction(string name, string value);
	int stoint(string value);
	bool stobool(string value);
public:
	ConfigHandler();
	virtual ~ConfigHandler();
	virtual void startDocument();
	virtual void endDocument();
	virtual void startElement(const XMLCh* const uri,
			const XMLCh* const localname, const XMLCh* const qname,
			const Attributes& attrs);
	virtual void characters(const XMLCh* const chars, unsigned int length);

	virtual void endElement(const XMLCh* const uri,
			const XMLCh* const localname, const XMLCh* const qname);
	Configuration *GetConfiguration();

};

#endif /* CONFIGHANDLER_H_ */
