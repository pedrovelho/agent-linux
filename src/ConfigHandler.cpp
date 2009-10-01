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
 * ConfigHandler.cpp
 *
 *  Created on: Apr 28, 2009
 *      Author: vasile
 */

#include "ConfigHandler.h"
ConfigHandler::ConfigHandler() {
	logger = log4cxx::Logger::getLogger("ConfigHandler");
	config = Configuration::Inst();
}

ConfigHandler::~ConfigHandler() {
	//unlink from the valid data
	//	currentEvent = new Event();
	//	currentAction = new Action();
	//	//delete
	//	delete currentEvent;
	//	delete currentAction;
}
void ConfigHandler::startDocument() {
	LOG4CXX_INFO(logger, "Started parsing file");
}
void ConfigHandler::endDocument() {
	LOG4CXX_INFO(logger, "Finished parsing file");
}
void ConfigHandler::characters(const XMLCh* const chars, unsigned int length) {
	element_data = XMLString::transcode(chars);
	LOG4CXX_TRACE(logger, "Found data:" + element_data);
}
void ConfigHandler::startElement(const XMLCh* const uri,
		const XMLCh* const localname, const XMLCh* const qname,
		const Attributes& attributes) {

	std::string name = XMLString::transcode(localname);
	//check type and create appropriate objects
	LOG4CXX_TRACE(logger, "Processing element [" << name << "] with ["
			<< attributes.getLength() << "] attributes");
	//found action
	if (name == INTERNAL_CONFIG) {
		LOG4CXX_DEBUG(logger, "#Found config");
		processing_config = true;
	}
	if (name == ACTION) {
		LOG4CXX_DEBUG(logger, "#Found action");
		processing_action = true;
		string action_name = XMLString::transcode(attributes.getValue(
				XMLString::transcode("xsi:type")));
		//create an object and process attributes depending on the event type
		if (action_name == ADVERT_ACTION) {
			currentAction = new AdvertAction();
		} else if (action_name == RM_ACTION) {
			currentAction = new RMAction();
		} else if (action_name == CUSTOM_ACTION) {
			currentAction = new CustomAction();
		} else if (action_name == P2P_ACTION) {
			currentAction = new P2PAction();
		}
		//iterate through attributes
		XMLSize_t len = attributes.getLength();
		for (XMLSize_t index = 0; index < len; index++) {
			LOG4CXX_DEBUG(logger, "Processing  attribute"
					<< XMLString::transcode(attributes.getURI(index))
					<< XMLString::transcode(attributes.getLocalName(index))
					<< XMLString::transcode(attributes.getQName(index))
					<< XMLString::transcode(attributes.getValue(index)));
			processAction(XMLString::transcode(attributes.getLocalName(index)),
					XMLString::transcode(attributes.getValue(index)));

		}
	}

	//found event
	if (name == EVENT) {
		processing_event = true;
		string event_name = XMLString::transcode(attributes.getValue(
				XMLString::transcode("xsi:type")));
		//create an object and process attributes depending on the event type
		if (event_name == CALENDAR_EVENT) {
			LOG4CXX_TRACE(logger, "#Found calendar event");
			currentEvent = new CalendarEvent();
		} else if (event_name == IDLENESS_EVENT) {
			LOG4CXX_TRACE(logger, "#Found idleness event");
			currentEvent = new IdlenessEvent();
		}
		//iterate through attributes
		XMLSize_t len = attributes.getLength();
		for (XMLSize_t index = 0; index < len; index++) {
			LOG4CXX_TRACE(logger, "Processing attribute "
					<< XMLString::transcode(attributes.getURI(index))
					<< XMLString::transcode(attributes.getLocalName(index))
					<< XMLString::transcode(attributes.getQName(index))
					<< XMLString::transcode(attributes.getValue(index)));
			processEvent(XMLString::transcode(attributes.getLocalName(index)),
					XMLString::transcode(attributes.getValue(index)));

		}
	}
}
void ConfigHandler::endElement(const XMLCh* const uri,
		const XMLCh* const localname, const XMLCh* const qname) {
	std::string name = XMLString::transcode(localname);
	LOG4CXX_DEBUG(logger, "Found end element [" << name << "]");

	//processing events
	if ((name == START_DAY || name == START_HOUR || name == START_MINUTE
			|| name == START_SECOND || name == DURATION_DAYS || name
			== DURATION_HOURS || name == DURATION_MINUTES || name
			== DURATION_SECONDS || name == PROCESS_PRIORITY || name
			== MAX_CPU_USAGE || name == BEGIN_SECONDS || name == END_SECONDS
			|| name == BEGIN_THRESHOLD || name == END_THRESHOLD || name
			== EVENT) && (processing_event)) {
		LOG4CXX_TRACE(logger, "Processing end element" << name
				<< " with value " << element_data);
		processEvent(name, element_data);
	}

	//processing actions
	if ((name == INITIAL_RESTART_DELAY || name == JAVA_STARTER_CLASS || name
			== IS_ENABLED || name == ACTION_ARGS || name == USERNAME || name
			== PASSWORD || name == PROTOCOL || name == URL || name == CONTACT
			|| name == ACTION || name == NODE_NAME) && (processing_action)) {
		LOG4CXX_TRACE(logger, "Processing end element " << name
				<< " with value " << element_data);
		processAction(name, element_data);
	}
	if (name == INTERNAL_CONFIG) {
		processing_config = false;
	}
	//configuration values are unique in the document, should be safe to do this
	if (processing_config) {
		if (name == PROACTIVE_LOCATION) {
			config->SetPALocation(element_data);
			LOG4CXX_TRACE(logger, "Set ProActive location to " + element_data);
		} else if (name == JAVA_HOME) {
			config->SetJavaHome(element_data);
			LOG4CXX_TRACE(logger, "Set Java home location to " + element_data);
		} else if (name == JAVA_PARAMS) {
			config->SetJVMParams(element_data);
			LOG4CXX_TRACE(logger, "Set JVM parameters to " + element_data);
		} else if (name == ENABLE_MEMORY_MNGMT) {
			config->setMemoryManagement(false);
			if (element_data == ENABLED)
				config->setMemoryManagement(true);
			LOG4CXX_TRACE(logger, "Set Memory management " + element_data);
		} else if (name == JAVA_MEMORY) {
			config->SetJavaMemory(stoint(element_data));
			LOG4CXX_TRACE(logger, "Set Java memory " + element_data);
		} else if (name == NATIVE_MEMORY) {
			config->SetNativeMemory(stoint(element_data));
			LOG4CXX_TRACE(logger, "Set native memory " + element_data);
		} else if (name == NO_PROCESSES) {
			config->SetNoProcesses(stoint(element_data));
			LOG4CXX_TRACE(logger, "Set no of processes " + element_data);
		} else if (name == USE_ALL_CPUS) {
			config->SetUseAllCPUs(stobool(element_data));
			LOG4CXX_TRACE(logger, "Set use all cpus to " + element_data);
		} else if (name == CONFIG_PROTOCOL) {
			config->SetConfigProtocol(element_data);
			LOG4CXX_TRACE(logger, "Set protocol to " + element_data);
		} else if (name == PORT_INITIAL_VALUE) {
			config->SetPortInitialValue(stoint(element_data));
			LOG4CXX_TRACE(logger, "Set port initial value to " + element_data);
		}
	}
}

//helper methods
int ConfigHandler::stoint(string value) {
	stringstream ss(value);

	int i;

	if ((ss >> i).fail()) {
		LOG4CXX_FATAL(logger, "Conversion of " << value << " to int failed");
		return 0;
	}
	return i;
}
bool ConfigHandler::stobool(string value) {
	transform(value.begin(), value.end(), value.begin(), ::tolower);
	if (value == ENABLED) {
		LOG4CXX_TRACE(logger, "Returning true: " << value);
		return true;
	} else {
		LOG4CXX_TRACE(logger, "Returning false : " << value);
		return false;
	}
}
void ConfigHandler::processEvent(string name, string value) {
	if (name == START_DAY) {
		((CalendarEvent*) currentEvent)->SetStartDay(value);
	} else if (name == START_HOUR) {
		((CalendarEvent*) currentEvent)->SetStartHour(stoint(value));
	} else if (name == START_MINUTE) {
		((CalendarEvent*) currentEvent)->SetStartMinute(stoint(value));
	} else if (name == START_SECOND) {
		((CalendarEvent*) currentEvent)->SetStartSecond(stoint(value));
	} else if (name == DURATION_DAYS) {
		((CalendarEvent*) currentEvent)->SetDurationDays(stoint(value));
	} else if (name == DURATION_HOURS) {
		((CalendarEvent*) currentEvent)->SetDurationHours(stoint(value));
	} else if (name == DURATION_MINUTES) {
		((CalendarEvent*) currentEvent)->SetDurationMinutes(stoint(value));
	} else if (name == DURATION_SECONDS) {
		((CalendarEvent*) currentEvent)->SetDurationSeconds(stoint(value));
	} else if (name == BEGIN_SECONDS) {
		((IdlenessEvent*) currentEvent)->SetBeginSeconds(stoint(value));
	} else if (name == END_SECONDS) {
		((IdlenessEvent*) currentEvent)->SetEndSeconds(stoint(value));
	} else if (name == BEGIN_THRESHOLD) {
		((IdlenessEvent*) currentEvent)->SetBeginThreshold(stoint(value));
	} else if (name == END_THRESHOLD) {
		((IdlenessEvent*) currentEvent)->SetEndThreshold(stoint(value));
	} else if (name == PROCESS_PRIORITY) {
		currentEvent->SetProcessPriority(value);
	} else if (name == MAX_CPU_USAGE) {
		currentEvent->SetMaxCPUUsage(stoint(value));
		//found end of an event, copying object to
		//the event vectors and destroying object
	} else if (name == EVENT) {
		config->AddEvent(currentEvent);
		//		delete currentEvent;
		LOG4CXX_DEBUG(logger,
				"#Added an event to the event vector in configuration");
		processing_event = false;
	}
}
void ConfigHandler::processAction(string name, string value) {
	if (name == INITIAL_RESTART_DELAY) {
		currentAction->SetRestartDelay(stoint(value));
	} else if (name == JAVA_STARTER_CLASS) {
		currentAction->SetStarterClass(value);
	} else if (name == IS_ENABLED) {
		currentAction->SetEnabled(stobool(value));
	} else if (name == NODE_NAME) {
		currentAction->SetNodeName(value);
	} else if (name == ACTION_ARGS) {
		((CustomAction*) currentAction)->SetArguments(value);
	} else if (name == USERNAME) {
		((RMAction*) currentAction)->SetUsername(value);
	} else if (name == PASSWORD) {
		((RMAction*) currentAction)->SetPassword(value);
	} else if (name == URL) {
		((RMAction*) currentAction)->SetURL(value);
	} else if (name == CONTACT) {
		((P2PAction*) currentAction)->SetContact(value);
	} else if (name == PROTOCOL) {
		((P2PAction*) currentAction)->SetProtocol(value);
	} else if (name == ACTION) {
		config->AddAction(currentAction);
		//		delete currentAction;
		LOG4CXX_DEBUG(logger,
				"#Added an action to the action vector in configuration");
		processing_action = false;
	}
}
Configuration *ConfigHandler::GetConfiguration() {
	return config;
}
