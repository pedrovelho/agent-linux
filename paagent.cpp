////============================================================================
//// Name        : bleah.cpp
//// Author      : Vasile Jureschi
//// Version     :
//// Copyright   : GPL-2
////============================================================================
#include <iostream>

#include <string>
#include "Runner.h"
#include "Constants.h"

using namespace std;
int main() {
//	Runner runner;
//	runner.start();
//	runner.join();
	cout << DEFAULT_JAVA_CLASS.c_str() << endl;
	int piexec = execl(DEFAULT_JAVA_BIN.c_str(), " ",
			DEFAULT_DSECURITY_MANAGER.c_str(),
			DEFAULT_DJAVA_SECURITY.c_str(),
			DEFAULT_DLOG4J_FILE.c_str(),
			DEFAULT_DPROACTIVE_HOME.c_str(),
			"-classpath", DEFAULT_DCLASSPATH.c_str(),
			DEFAULT_JAVA_CLASS.c_str(), "node1", (char *) 0);


}


