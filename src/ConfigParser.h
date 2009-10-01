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
 * ConfigParser.h
 *
 *  Created on: Apr 27, 2009
 *      Author: vasile
 */

#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_
#include <string>
#include <vector>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
using namespace xercesc;
using namespace std;
#include "ConfigHandler.h"
#include "Event.h"


class ConfigParser {
public:
	ConfigParser(std::string config_path);
	virtual ~ConfigParser();
	Configuration *GetConfiguration();
private:
	std::string xmlFile;
	SAX2XMLReader* parser;
	ConfigHandler* defaultHandler;
	std::vector<Event> events;
	LoggerPtr logger;
};

#endif /* CONFIGPARSER_H_ */
