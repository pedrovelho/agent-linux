/*
 * paagent_server.cpp
 *
 *  Created on: Sep 4, 2009
 *      Author: vasile
 */

#include <iostream>

#include <string>
#include "PAAgent.h"
using namespace std;
int main() {
	PAAgent agent;
	agent.start();
	agent.join();
}

