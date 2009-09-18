#ifndef RUNNER_H
#define RUNNER_H

#include "Configuration.h"
#include <string>
#include <time.h>
#include <iostream>
#include <cc++/thread.h>
#include "ConfigParser.h"
#include "Watcher.h"
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "controller_proxy.h"

using namespace ost;

class Runner : public Thread{
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
	void StartActions();

private:
	Configuration* configuration;
	LoggerPtr logger;
	vector<Watcher*> watchers;
protected:
	virtual void run();

};

#endif // RUNNER_H
