/*
 * Watcher.cpp
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#include "Watcher.h"
Watcher::Watcher() {
}

void Watcher::Start(char* shell, char* node_executable,
		char* node_name, int jvm_pid){
		int watcher_pid = fork();
		bool stop = false;
		cout << "Watcher forked with pid :" << watcher_pid << endl;
			//run in the child process
			if (watcher_pid == 0 ){
				umask(0);
				cout << "Checking "<< jvm_pid << " with an interval of "
					<< GetTick(jvm_pid) << endl;
				while (!stop){
					//usleep takes microseconds, we use milliseconds
					cout << "Checking "<< jvm_pid << " with an interval of "
								<< GetTick(jvm_pid) << endl;
					usleep(GetTick(jvm_pid)*1000);
					// if a signal cannot be sent - return is -1
					// start the jvm again
					if (kill(jvm_pid, 0) == -1 ) {
						cout << "Node " <<  jvm_pid  << "has been killed" << endl;
						cout << "Signaling the controller and stopping thread..." << endl;
						stop = true;
						//restarting JVM
						StartWatcher(shell, node_executable ,node_name,
								StartNode(shell, node_executable, node_name));
						exit(0);
					}
				}
			}
			else{ cout << "In else" <<endl;}
			//associate node name with watcher pid
			//this will also run when the node dies and is restarted,
			//however it is not an issue as the node name should not change
			//between restarts

			//sets watcher pid in the parent process
			SetWatcherPid(jvm_pid, watcher_pid);
}


Watcher::~Watcher() {
	// TODO Auto-generated destructor stub
}
