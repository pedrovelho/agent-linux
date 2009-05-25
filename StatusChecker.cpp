/*
 * StatusChecker.cpp
 *
 *  Created on: Mar 19, 2009
 *      Author: vasile
 */


#include "StatusChecker.h"

StatusChecker::StatusChecker() {
//	jvm_pids = new vector<int>;
}

StatusChecker::~StatusChecker() {
	// TODO Auto-generated destructor stub
}

void StatusChecker::AddJVM(int jvm_pid) {
	//TODO add check for existing pid ?
	jvm_pids.push_back(jvm_pid);
}

bool StatusChecker::RemoveJVM(int jvm_pid) {
	int size = jvm_pids.size();
	/* run through the vector and remove
	 * if the pid exists */
    for (long index=0; index<size; ++index) {
        try {
            if (jvm_pids.at(index) == jvm_pid) {
            	jvm_pids.erase(jvm_pids.begin()+index);
            	return true;
            }
        }
        catch (exception& e) {
            cerr  << index << ": index exceeds vector dimensions." << endl;
//          cerr << e;
        }
    }
    return false;
}
/**
 * Checks if a process is running by verifying
 * if the /proc/pid exists.
 */
bool StatusChecker::IsRunning(int jvm_pid) {
	//convert to string bull
	std::string pid;
	std::stringstream out;
	out << jvm_pid;
	pid = out.str();
	string process_path("/proc/");
	process_path = process_path + pid;
	struct stat stFileInfo;
	bool pid_exists;
	int intStat;

	// Attempt to get the file attributes
	intStat = stat(process_path.c_str(),&stFileInfo);
	if(intStat == 0) {
		// We were able to get the file attributes
		// so the file obviously exists.
		pid_exists = true;
	} else {
		// We were not able to get the file attributes.
		// This may mean that we don't have permission to
		// access the folder which contains this file. If you
		// need to do that level of checking, lookup the
		// return values of stat which will give you
		// more details on why stat failed.
		pid_exists = false;
	}
	return pid_exists;
}
vector<int> StatusChecker::GetJVMs() {
	return jvm_pids;
}

vector<int> StatusChecker::GetStoppedJVMs() {
	vector <int> stopped;
	int size = jvm_pids.size();
	/* run through the vector and
	 * and add to the list the stoped jvms
	 */
    for (long index=0; index<size; ++index) {
        try {
            if (IsRunning(jvm_pids.at(index)) ) stopped.push_back(jvm_pids.at(index));
        }
        catch (exception& e) {
            cerr  << index << ": index exceeds vector dimensions." << endl;
//          cerr << e;
        }
    }
    return stopped;
}

vector<int> StatusChecker::GetRunningJVMs() {
	vector <int> running;
	int size = jvm_pids.size();
	/* run through the vector and
	 * and add to the list the running jvms
	 */
    for (long index=0; index<size; ++index) {
        try {
            if (IsRunning(jvm_pids.at(index)) ) running.push_back(jvm_pids.at(index));
        }
        catch (exception& e) {
            cerr  << index << ": index exceeds vector dimensions." << endl;
//          cerr << e;
        }
    }
    return running;
}
void StatusChecker::RemoveAllJVMs(){
	jvm_pids.empty();
}
