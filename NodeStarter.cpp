/*
 * NodeStarter.cpp
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#include "NodeStarter.h"

NodeStarter::NodeStarter() {
	// TODO Auto-generated constructor stub

}

NodeStarter::~NodeStarter() {
	// TODO Auto-generated destructor stub
}

void NodeStarter::Execute(void *arg){

	char *shell = "/bin/bash";
	char *node_exec = "/home/vasile/workspacePA/PA_new/trunk/bin/startNode.sh";
	char *node_name = "node1";

	int piexec =  execlp(shell, "-c", node_exec,
				node_name, (char *) 0);
	cout << piexec;
}
