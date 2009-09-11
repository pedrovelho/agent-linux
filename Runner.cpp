#include "Runner.h"
// Constructors/Destructors
//
#define SCANFPATTERN "%4s %lf %lf %lf %lf %lf %lf %lf"
#define PRINTF "%3.1lf%%"
Runner::Runner() {
	logger = log4cxx::Logger::getLogger("Controller");
	//	BasicConfigurator::configure();
	logger->setLevel(log4cxx::Level::getTrace());
}

Runner::~Runner() {
}
void Runner::run() {
	ConfigParser parser("./ConfigExample.xml");
	configuration = parser.GetConfiguration();

	CalendarEvent *calendar;
	IdlenessEvent *idle;
	int sleep_time = 1; //second value
	int passes = 0;
	//	while (1) {
	//
	//		//*********read cpu ticks
	//		int num = 7;
	//
	//		FILE *f1 = fopen("/proc/stat", "r");
	//
	//		double ticks1[7];
	//		char label1[5];
	//
	//		int vals1 = fscanf(f1, SCANFPATTERN, label1, &ticks1[0], &ticks1[1],
	//				&ticks1[2], &ticks1[3], &ticks1[4], &ticks1[5], &ticks1[6]);
	//
	//		fclose(f1);
	//
	//		vector<CalendarEvent*> events = configuration->getCalendar_events();
	//		int no_calendars = events.size();
	//		for (int i = 0; i < no_calendars; i++) {
	//			calendar = events.at(i);
	//			//start event if the time matches
	//			if (isNow(calendar)) {
	//				events_duration.push_back(calendar->GetTotalDuration());
	//				cout << "++++++++++++STARTING NOW";
	//			}
	//		}
	//
	//		//stop event if the time is up
	//		int k = 0;
	//		while (k < events_duration.size()) {
	//			if (events_duration.at(k) < 0) {
	//				cout << "++++++++++++STOPPING NOW";
	//				events_duration.erase(events_duration.begin() + k);
	//				// don't increment  k because erasing the k
	//				// element moves all the elements to the left
	//			} else {
	//				k++;
	//			}
	//
	//		}
	//
	//		//sleep takes milliseconds
	//		sleep(sleep_time * 1000);
	//
	//		//decrement time for event duration equal to sleep time
	//		//durations are in seconds
	//		for (int i = 0; i < events_duration.size(); i++) {
	//			events_duration.at(i) = events_duration.at(i) - sleep_time;
	//		}
	//
	//		double idleness = 10000; //making sure it is bigger than end threshold
	//		//read cpu ticks again but wait for 10 passes
	//		//to get more accurate readings
	//		if (passes > 10) {
	//			FILE *f2 = fopen("/proc/stat", "r");
	//			double ticks2[7];
	//			char label2[5];
	//			int vals2 = fscanf(f2, SCANFPATTERN, label2, &ticks2[0],
	//					&ticks2[1], &ticks2[2], &ticks2[3], &ticks2[4], &ticks2[5],
	//					&ticks2[6]);
	//			fclose(f2);
	//			cout << "Idle :" << (ticks2[3] - ticks1[3]) / 2 << "%" << endl;
	//			passes = 0;
	//
	//			idleness = (ticks2[3] - ticks1[3]) / 2;
	//			//test idle events with the
	//			//idle value calculated
	//
	//		}
	//		passes++;
	//		vector<IdlenessEvent*> idle_events = configuration->getIdle_events();
	//		for (int i = 0; i < idle_events.size(); i++) {
	//			IdlenessEvent * idle = idle_events.at(i);
	//			//FIXME why a begin and end threshold ?
	//			//why not one idleness limit ?
	//			//doesn't make sense
	//
	//			//if idleness is between thresholds
	//			if ((idle->GetBeginThreshold() < idleness)
	//					&& (idle->GetEndThreshold() > idleness)) {
	//				//start counting seconds
	//
	//			}
	//
	//		}
	//
	//	}

	int idleness = 0;
	FILE *f;
	//init flags
	bool cal_event_on = false;
	bool idle_event_on = false;
	bool stop_actions = false;
	//init counter
	long cal_duration = 0;
	//init vectors
	vector<CalendarEvent*> cal_events = configuration->getCalendar_events();
	vector<IdlenessEvent*> idle_events = configuration->getIdle_events();
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
					LOG4CXX_DEBUG(logger, "++++++++++++STOPPING CALENDAR NOW");
					cal_event_on = false;
					stop_actions = true;
				} else {
					//decrease timer for running calendar event
					events_duration.at(i)= events_duration.at(i) - sleep_time;
					LOG4CXX_TRACE(logger, "Decreasing timer for calendar event " <<
							events_duration.at(i) << " seconds left");

				}
			}
		} else if (idle_event_on) {
			LOG4CXX_TRACE(logger, "Checking if idleness events have to be stopped")
			idle_events = configuration->getIdle_events();
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

				//if start_counter is < 1 start idleness event
				//because the CPU has been between thresholds for the required time
				if (stop_counter.at(i) < 1) {
					//TODO start JVMS
					LOG4CXX_DEBUG(logger, "++++++++++STOPPING IDLE EVENT");
					idle_event_on = false;
					stop_actions = true;
				}
			}//for
			;
			//increaser timer for stop checks

		}

		if (stop_actions) {
			//TODO stop actions
			LOG4CXX_DEBUG(logger," ++++++++++STOPPING ACTIONS");
		}

		//check for  calendar events to start
		cal_events = configuration->getCalendar_events();
		for (int i = 0; i < cal_events.size(); i++) {
			LOG4CXX_TRACE(logger, "Checking for calendar events to start");
			calendar = cal_events.at(i);
			//start event if the time matches
			if (isNow(calendar)) {
				events_duration.at(i) = calendar->GetTotalDuration();
				//check if an idle event is on and pass the control to the calendar event
				if (idle_event_on) {
					idle_event_on = false;
					//TODO copy settings of idle events started JVMS,etc and pass them to the calendar event
				}
				cal_event_on = true;
				LOG4CXX_DEBUG(logger, "++++++++++STARTING CALENDAR EVENT with duration of "<<
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
					//TODO start JVMS
					LOG4CXX_DEBUG(logger,"++++++++++STARTING IDLE EVENT");
					idle_event_on = true;
				}
			}//for
		}//if (!cal_event_on || !idle_event_on)

	}
}
/**
 * Get the month, day of the month, and year to
 * be able to construct a string to compare with
 * the current time.
 */
bool Runner::isNow(CalendarEvent *calendar) {
	//used for easy int->string conversion;
	stringstream out;

	string month;
	string year;
	string day_of_month;

	//get time
	time_t my_time = time(NULL);
	string now = ctime(&my_time);
	string now_bkp = now;

	//remove the day of week
	now = now.substr(now.find_first_of(" ", 0) + 1, now.length());

	//get the month
	month = now.substr(0, now.find_first_of(" ", 0));
	//remove the month
	now = now.substr(now.find_first_of(" ", 0) + 1, now.length());
	//get the day of the month
	day_of_month = now.substr(0, now.find_first_of(" ", 0));
	//get the year

	year = now.substr(now.length() - 5, now.length());

	//first three characters of week day to lowercase
	//taken from calendarevent time


	string calendar_weekday = calendar->GetStartDay();
	calendar_weekday = calendar_weekday.substr(0, 3);

	out << calendar_weekday << " " << month << " " << day_of_month << " ";
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
	out << calendar->GetStartSecond() << " ";
	out << year;

	string calendar_time = out.str();
	transform(calendar_time.begin(), calendar_time.end(),
			calendar_time.begin(), ::tolower);
	transform(now_bkp.begin(), now_bkp.end(), now_bkp.begin(), ::tolower);
	LOG4CXX_TRACE(logger, "Configuration time is :" << calendar_time);
	LOG4CXX_TRACE(logger, "Current time is       :" << now_bkp);
	if (calendar_time == now_bkp) {
		return true;
	}
	return false;
}

/**
 * Set the value of configuration
 * @param new_var the new value of configuration
 */
void Runner::setConfiguration(Configuration* new_var) {
	configuration = new_var;
}

/**
 * Get the value of configuration
 * @return the value of configuration
 */
Configuration* Runner::getConfiguration() {
	return configuration;
}

