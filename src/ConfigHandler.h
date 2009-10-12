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
namespace paagent {
/**
 * Standard SAX parser for the configuration file.
 */
class ConfigHandler: public DefaultHandler {
private:
	LoggerPtr logger;
	/** The current element data, this changes with every element.*/
	string element_data;
	/** The Configuration object to which the parsed data is added.*/
	Configuration *config;
	Event *currentEvent;
	Action *currentAction;
	/** True if an event tag has been encountered and we haven't found the end tag yet.*/
	bool processing_event;
	/** True if an action tag has been encountered and we haven't found the end tag yet.*/
	bool processing_action;
	/** True if an internalConfig tag has been encountered and we haven't found the end tag yet.*/
	bool processing_config;
	/**
	 * The method adds the value to the current Event object
	 * depending on the tag name. If an end event tag is found
	 * then the current Event object is added to the vector in
	 * the Configuration object.
	 * @param name the name of the tag being processed
	 * @param value the data for the tag being processed
	 * */
	void processEvent(string name, string value);
	/**
	 * The method adds the value to the current Action object
	 * depending on the tag name. If an end action tag is found
	 * then the current Action object is added to the vector in
	 * the Configuration object.
	 * @param name the name of the tag being processed
	 * @param value the data for the tag being processed
	 * */
	void processAction(string name, string value);
	/** Helper method for converting string to int values.
	 * @param value the string to be converted
	 * */
	int stoint(string value);
	/** Helper method for converting string to bool values.
	 * @param value the string to be converted
	 * */
	bool stobool(string value);
public:
	ConfigHandler();
	virtual ~ConfigHandler();
	virtual void startDocument();
	virtual void endDocument();
	/**
	 * The method looks for either configuration, event,
	 * or action tags and sets the handling mode to the
	 * found tag. If the tags have attributes than it iterates
	 * through the attributes calling either processEvent or processAction
	 * depending on the current handling mode. Depending on the
	 * Action and Event type it creates an appropriate Event or
	 * Action object to added to the event/action vectors in
	 * the Configuration object when an end event/action tag
	 * is encountered.
	 */
	virtual void startElement(const XMLCh* const uri,
			const XMLCh* const localname, const XMLCh* const qname,
			const Attributes& attrs);
	virtual void characters(const XMLCh* const chars, unsigned int length);
	/**
	 * The method looks for action, event, or internal config end tags
	 * and for the inner tags of these tags and processes them using
	 * processEvent or processAction. If an action, event, or internal config
	 * tag is found then it sets the corresponding processing mode to false.
	 */
	virtual void endElement(const XMLCh* const uri,
			const XMLCh* const localname, const XMLCh* const qname);
	Configuration *GetConfiguration();

};
} //namespace paagent
#endif /* CONFIGHANDLER_H_ */
