/*
 * StatusChecker.h
 *
 *  Created on: Mar 19, 2009
 *      Author: vasile
 */

#ifndef STATUSCHECKER_H_
using namespace std;
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <vector>
#define STATUSCHECKER_H_

class StatusChecker {
private:
	//JVMs  processes ids
	vector<int> jvm_pids;
public:
	StatusChecker();
	virtual ~StatusChecker();
	bool IsRunning(int);
	void AddJVM(int);
	bool RemoveJVM(int);
	void RemoveAllJVMs();
	vector<int> GetJVMs();
	vector<int> GetStoppedJVMs();
	vector<int> GetRunningJVMs();
 	//TODO set a tick for check and a reference to a method to be called
	//TODO enable disable tick
};

#endif /* STATUSCHECKER_H_ */
