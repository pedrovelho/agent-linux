#ifndef RUNNER_H
#define RUNNER_H

#include "Configuration.h"
#include <string>
#include <time.h>
#include <iostream>
#include <cc++/thread.h>
#include "ConfigParser.h"

#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace ost;

class Runner: public Thread {
public:
	Runner();
	virtual ~Runner();

	/**
	 */
	void Stop() {
	}

	/**
	 */
	void ReloadConfiguration() {
	}
	bool isNow(CalendarEvent *calendar);
	void setConfiguration(Configuration* new_var);
	Configuration* getConfiguration();
private:
	Configuration* configuration;
	LoggerPtr logger;
protected:
	virtual void run();

};

#endif // RUNNER_H
