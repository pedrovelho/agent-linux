////============================================================================
//// Name        : bleah.cpp
//// Author      : Vasile Jureschi
//// Version     :
//// Copyright   : GPL-2
////============================================================================
#include <iostream>

#include <string>
#include "ConfigParser.h"
using namespace std;
int main() {
	ConfigParser parser("/home/vasile/workspacePA/paagent/ConfigExample.xml");
	parser.startNodes();
}

