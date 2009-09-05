/*
 * Event.h
 *
 *  Created on: May 11, 2009
 *      Author: vasile
 */

#ifndef EVENT_H_
#define EVENT_H_
#include <string>
using namespace std;
class Event {
public:
	Event();
	virtual ~Event();
	void SetProcessPriority(string priority);
	void SetMaxCPUUsage(int max);
	string GetProcessPriority();
	int GetMaxCPUUsage();
private:
	string process_priority;
	int max_cpu_usage;
};

#endif /* EVENT_H_ */
