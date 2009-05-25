/*
 * Watcher.h
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#ifndef GENERICS
#define GENERICS
using namespace std;
#include <signal.h>
#include <spawn.h>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>

#endif

#ifndef WATCHER_H_
#define WATCHER_H_

class Watcher {
private:
	int pid;
	int tick;
	bool stop;
public:
	Watcher();
	int GetTick();
	int GetPid();
	void SetTick();
	void SetPid();
	void Start(char* shell, char* node_executable,
			char* node_name, int jvm_pid);
	virtual ~Watcher();
};

#endif /* WATCHER_H_ */
