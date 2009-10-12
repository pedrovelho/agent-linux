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

#include "Runner.h"
namespace paagent {
#define SCANFPATTERN "%4s %lf %lf %lf %lf %lf %lf %lf"
Runner::Runner(string xml_file) {
	logger = log4cxx::Logger::getLogger("Runner");
	config_file = xml_file;

}
Runner::~Runner() {
	//delete watchers
	watchers.clear();
}
void Runner::run() {
	//FIXME placing this in constructor does not work, why?
	DBus::init(true);
	DBus::Dispatcher dispatcher;
	DBus::Connection::pointer connection = dispatcher.create_connection(
			DBus::BUS_SESSION);
	DBus::ControllerProxy::pointer controller = DBus::ControllerProxy::create(
			connection);

	//initialize DBus Connection
	ConfigParser parser(config_file);

	configuration = parser.GetConfiguration();

	CalendarEvent *calendar;

	IdlenessEvent *idle;

	int sleep_time = 1; //second value

	int passes = 0;

	int idleness = 0;

	FILE *f;
	//init flags
	bool cal_event_on = false;
	bool idle_event_on = false;
	bool stop_actions = false;
	//init counter
	long cal_duration = 0;
	//init vectors
	vector<CalendarEvent*> cal_events = configuration->GetCalendarEvents();
	vector<IdlenessEvent*> idle_events = configuration->GetIdleEvents();
	//init time checks vector for Idleness event start or stop
	vector<int> start_counter;
	vector<int> stop_counter;
	for (int i = 0; i < idle_events.size(); i++) {
		idle = idle_events.at(i);
		start_counter.push_back(idle->GetBeginSeconds());
		stop_counter.push_back(idle->GetEndSeconds());
		LOG4CXX_TRACE(logger, "Initialized start/stop: " << start_counter.at(i) << "," << stop_counter.at(i) );
	}
	//init duration vector
	vector<long> events_duration(cal_events.size(), 0);
	//Assuming (even if it checks for several events):
	//1. calendar/idle events DO NOT OVERLAP
	//2. calendar events have precedence over idle events
	//3. there is only 1 event running at the time (although the code is bit more general)
	//4. all events start the same actions
	//5? time taken in loop, except for sleep is negligible

	while (1) {
		//--------- CPU utilization check
		int num = 7;
		f = fopen("/proc/stat", "r");
		double ticks1[7];
		char label1[5];
		int vals1 = fscanf(f, SCANFPATTERN, label1, &ticks1[0], &ticks1[1],
				&ticks1[2], &ticks1[3], &ticks1[4], &ticks1[5], &ticks1[6]);
		fclose(f);

		//sleep takes milliseconds
		//sleep here to get CPU utilization before logic
		sleep(sleep_time * 1000);

		f = fopen("/proc/stat", "r");
		double ticks2[7];
		char label2[5];
		int vals2 = fscanf(f, SCANFPATTERN, label2, &ticks2[0], &ticks2[1],
				&ticks2[2], &ticks2[3], &ticks2[4], &ticks2[5], &ticks2[6]);
		fclose(f);
		idleness = (ticks2[3] - ticks1[3]) / configuration->GetNoCPUS();
		LOG4CXX_TRACE(logger,"CPU idleness:" << idleness);
		//------------


		//check if anything is running and if it should be stopped
		//WARNING: this decrease all the values in the vector,even the ones for non running calendar events
		//because the running flag is global
		//it is not an issue because assumption 1 & 3 and because on starting a calendar event the
		//duration is set again
		stop_actions = false;
		if (cal_event_on) {
			LOG4CXX_TRACE(logger, "Checking if calendar events have to be stopped")
			//check if calendar event has finished and set stop actions
			//stop event if the time is up
			for (int i = 0; i < events_duration.size(); i++) {
				if (events_duration.at(i) < 1) {
					LOG4CXX_INFO(logger, "++++++++++++STOPPING CALENDAR NOW");
					cal_event_on = false;
					stop_actions = true;
				} else {
					//decrease timer for running calendar event
					events_duration.at(i) = events_duration.at(i) - sleep_time;
					LOG4CXX_TRACE(logger, "Decreasing timer for calendar event " <<
							events_duration.at(i) << " seconds left");

				}
			}
		} else if (idle_event_on) {
			LOG4CXX_TRACE(logger, "Checking if idleness events have to be stopped")
			idle_events = configuration->GetIdleEvents();
			for (int i = 0; i < idle_events.size(); i++) {
				idle = idle_events.at(i);
				//FIXME why a begin and end threshold ?
				//why not one idleness limit ?
				//doesn't make sense

				//if idleness is between thresholds decrease stop counter otherwise reset the stop counter
				if ((idle->GetBeginThreshold() > idleness)
						|| (idle->GetEndThreshold() < idleness)) {
					//decrease start counter
					stop_counter.at(i) = stop_counter.at(i) - sleep_time;
					LOG4CXX_TRACE(logger, "Decreasing stop counter for idle event " << stop_counter.at(i)
							<< " seconds. Thresholds are: [" <<idle->GetBeginThreshold() <<
							"," <<idle->GetEndThreshold() <<"]" );
				} else {
					LOG4CXX_TRACE(logger, "Resetting timer for idleness event");
					stop_counter.at(i) = idle->GetBeginSeconds();
				}

				//if stop_counter is < 1 stop idleness events
				//because the CPU has been between thresholds for the required time
				if (stop_counter.at(i) < 1) {
					LOG4CXX_INFO(logger, "++++++++++STOPPING IDLE EVENT");
					idle_event_on = false;
					stop_actions = true;
				}
			}//for
			;
			//increaser timer for stop checks

		}

		if (stop_actions) {
			LOG4CXX_INFO(logger,"++++++++++++STOPPING ACTIONS");
			StopActions(controller);
		}

		//check for  calendar events to start
		cal_events = configuration->GetCalendarEvents();
		for (int i = 0; i < cal_events.size(); i++) {
			LOG4CXX_TRACE(logger, "Checking for calendar events to start");
			calendar = cal_events.at(i);
			//start event if the time matches
			if (isNow(calendar)) {
				events_duration.at(i) = calendar->GetTotalDuration();
				//check if an idle event is on and pass the control to the calendar event
				//if no idle event is on (no actions have been started) start the actions
				if (idle_event_on) {
					LOG4CXX_INFO(logger, "++++++++++SWITCHING FROM IDLE EVENT TO CALENDAR EVENT");
					idle_event_on = false;
				}
				StartActions(controller, calendar->GetMaxCPUUsage());

				cal_event_on = true;
				LOG4CXX_INFO(logger, "++++++++++STARTING CALENDAR EVENT with duration of "<<
						events_duration.at(i) << " seconds");

			}
		}//for

		//check for idle events to start if no  events are running
		//idle event assumptions:
		//1. the CPU idleness must be between/out of threshold values for the specified amount of time
		//   to start/stop
		if (!cal_event_on && !idle_event_on) {
			LOG4CXX_TRACE(logger, "Checking for events to start");
			for (int i = 0; i < idle_events.size(); i++) {
				idle = idle_events.at(i);
				//FIXME why a begin and end threshold ?
				//why not one idleness limit ?
				//doesn't make sense

				//if idleness is between thresholds decrease counter otherwise reset the start counter
				if ((idle->GetBeginThreshold() < idleness)
						&& (idle->GetEndThreshold() > idleness)) {
					//decrease start counter
					start_counter.at(i) = start_counter.at(i) - sleep_time;
					LOG4CXX_TRACE(logger, "Decreasing start counter for idle event " << start_counter.at(i)
							<< " seconds. Thresholds are: [" <<idle->GetBeginThreshold() <<
							"," <<idle->GetEndThreshold() <<"]" );
				} else {
					LOG4CXX_TRACE(logger, "Resetting start counter for idle event" );
					start_counter.at(i) = idle->GetBeginSeconds();
				}

				//if start_counter is < 1 start idleness event
				//because the CPU has been between thresholds for the required time
				if (start_counter.at(i) < 1) {
					LOG4CXX_INFO(logger,"++++++++++STARTING IDLE EVENT");
					idle_event_on = true;
					StartActions(controller, calendar->GetMaxCPUUsage());
				}
			}//for
		}//if (!cal_event_on || !idle_event_on)
	}
}
/**
 * Extract parts of the date that are not in the configuration (month,
 * weekday,  hour, min, sec
 * */
bool Runner::isNow(CalendarEvent *calendar) {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[15];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 15, "%a %X", timeinfo);
	stringstream out;

	string calendar_weekday = calendar->GetStartDay();
	calendar_weekday = calendar_weekday.substr(0, 3);

	out << calendar_weekday << " ";
	if (calendar->GetStartHour() < 10) {
		out << "0";
	}
	out << calendar->GetStartHour() << ":";
	if (calendar->GetStartMinute() < 10) {
		out << "0";
	}
	out << calendar->GetStartMinute() << ":";
	if (calendar->GetStartSecond() < 10) {
		out << "0";
	}
	out << calendar->GetStartSecond();

	string calendar_time = out.str();
	transform(calendar_time.begin(), calendar_time.end(),
			calendar_time.begin(), ::tolower);
	string now(buffer);
	transform(now.begin(), now.end(), now.begin(), ::tolower);
	LOG4CXX_TRACE(logger, "Configuration time is :" << calendar_time);
	LOG4CXX_TRACE(logger, "Current time is       :" << now);
	if (calendar_time == now) {
		return true;
	}
	return false;
}

void Runner::setConfiguration(Configuration* config) {
	configuration = config;
}

/**
 * Get the value of configuration
 * @return the value of configuration
 */
Configuration* Runner::getConfiguration() {
	return configuration;
}

/**
 * check which actions are enabled and start them
 *
 * Assuming that an event starts all the enabled actions and
 * a stop event also stops all the actions
 *
 */
//TODO DRY much ?
void Runner::StartActions(DBus::ControllerProxy::pointer controller, int limit) {

	controller->SetStartConfiguration(configuration->GetJavaSecurityPolicy(),
			configuration->GetLog4jFile(), configuration->GetPALocation(),
			configuration->GetClasspath(), configuration->GetJavaHome()+DEFAULT_JAVA_BIN);
	controller->SetJVMSettings("", configuration->IsMemoryManaged(),
			configuration->GetJavaMemory(), configuration->GetNativeMemory(),
			configuration->GetNoProcesses(), configuration->UseAllCPUs());
	controller->SetNetworkSettings(configuration->GetPortInitialValue(),
			configuration->GetConfigProtocol());

//	LOG4CXX_TRACE(logger, "Set start configuration for controller to : [" <<
//			java_security << "] [" <<
//			log4j_configuration<< "] [" <<
//			pa_home_option<< "] [" <<
//			configuration->GetClasspath()<< "] [" <<
//			java_bin << "]");
	vector<AdvertAction*> advert_actions = configuration->GetAdvertActions();
	vector<RMAction*> rm_actions = configuration->GetRMActions();
	vector<P2PAction*> p2p_actions = configuration->GetP2PActions();
	vector<CustomAction*> custom_actions = configuration->GetCustomActions();

	int pid;
	AdvertAction *advert;

	for (int i = 0; i < advert_actions.size(); i++) {
		advert = advert_actions.at(i);
		if (advert->IsEnabled()) {
			//start and add the pid to the pid vector
			pid = controller->StartNode(advert->GetNodeName(),
					advert->GetStarterClass());
			//FIXME if watchers are initialized by static methods the
			//thread stops after a call to StartNode in controller
			//creating a watcher using a regular constructor doesn't seem to have
			//this problem
			//			Watcher *watcher = Watcher::AdvertWatcher(pid, DEFAULT_TICK, advert->GetRestartDelay(),
			//					advert->GetNodeName(), advert->GetStarterClass(), controller);
			Watcher *watcher = new Watcher(pid, DEFAULT_TICK,
					advert->GetRestartDelay(), advert->GetNodeName(),
					advert->GetStarterClass(), controller, ADVERT);
			//			//FIXME check the node has been actually started !!!
			LOG4CXX_TRACE(logger, "Advert node started " << advert->GetNodeName() );
			watcher->start();
			watcher->limit(limit);
			watchers.push_back(watcher);

		}
	}

	RMAction *rm;

	for (int i = 0; i < rm_actions.size(); i++) {
		rm = rm_actions.at(i);
		if (rm->IsEnabled()) {
			//start and add the pid to the pid vector
			pid = controller->StartRMNode(rm->GetNodeName(),
					rm->GetStarterClass(), rm->GetUsername(),
					rm->GetPassword(), rm->GetURL());
			//FIXME if watchers are initialized by static methods the
			//thread stops after a call to StartNode in controller
			//creating a watcher using a regular constructor doesn't seem to have
			//this problem
			//			Watcher *watcher = Watcher::RMWatcher(pid, DEFAULT_TICK, rm->GetRestartDelay(),
			//					rm->GetNodeName(), rm->GetStarterClass(), controller);
			Watcher *watcher = new Watcher(pid, DEFAULT_TICK,
					rm->GetRestartDelay(), rm->GetNodeName(),
					rm->GetStarterClass(), controller, RM);
			watcher->SetRMValues(rm->GetUsername(), rm->GetPassword(),
					rm->GetURL());
			LOG4CXX_TRACE(logger, "RM node started " << rm->GetNodeName() );
			watcher->start();
			watcher->limit(limit);
			watchers.push_back(watcher);

		}
	}

	P2PAction *p2p;

	for (int i = 0; i < p2p_actions.size(); i++) {
		p2p = p2p_actions.at(i);
		if (p2p->IsEnabled()) {
			//start and add the pid to the pid vector
			pid = controller->StartP2PNode(p2p->GetNodeName(),
					p2p->GetStarterClass(), p2p->GetContact());
			//FIXME if watchers are initialized by static methods the
			//thread stops after a call to StartNode in controller
			//creating a watcher using a regular constructor doesn't seem to have
			//this problem
			//			Watcher *watcher = Watcher::P2PWatcher(pid, DEFAULT_TICK, p2p->GetRestartDelay(),
			//					p2p->GetNodeName(), p2p->GetStarterClass(), controller);
			Watcher *watcher = new Watcher(pid, DEFAULT_TICK,
					p2p->GetRestartDelay(), p2p->GetNodeName(),
					p2p->GetStarterClass(), controller, P2P);
			watcher->SetP2PValues(p2p->GetContact());
			LOG4CXX_TRACE(logger, "P2P node started " << p2p->GetNodeName() );
			watcher->start();
			watcher->limit(limit);
			watchers.push_back(watcher);
		}
	}

	CustomAction *custom;

	for (int i = 0; i < custom_actions.size(); i++) {
		custom = custom_actions.at(i);
		if (custom->IsEnabled()) {
			//start and add the pid to the pid vector
			pid = controller->StartCustomNode(custom->GetNodeName(),
					custom->GetStarterClass(), custom->GetArguments());
			//FIXME if watchers are initialized by static methods the
			//thread stops after a call to StartNode in controller
			//creating a watcher using a regular constructor doesn't seem to have
			//this problem
			//			Watcher *watcher = Watcher::CustomWatcher(pid, DEFAULT_TICK, custom->GetRestartDelay(),
			//					custom->GetNodeName(), custom->GetStarterClass(), controller);
			Watcher *watcher = new Watcher(pid, DEFAULT_TICK,
					custom->GetRestartDelay(), custom->GetNodeName(),
					custom->GetStarterClass(), controller, CUSTOM);
			watcher->SetCustomValues(custom->GetArguments());
			LOG4CXX_TRACE(logger, "Custom node started " << custom->GetNodeName() );
			watcher->start();
			watcher->limit(limit);
			watchers.push_back(watcher);
		}
	}

}

void Runner::StopActions(DBus::ControllerProxy::pointer controller) {
	int pid;
	Watcher *watcher;
	for (int i = 0; i < watchers.size(); i++) {
		watcher = watchers.at(i);
		pid = watcher->GetPid();
		watcher->StopWatcher();
		watcher->join();
		LOG4CXX_TRACE(logger, "Found a watcher for node " << watcher->getName() << " with PID " << pid);
		cout << controller->StopNode(pid) << endl;
	}
}

void Runner::SetConfigurationFile(string xml_file) {
	config_file = xml_file;
}
} //namespace paagent
