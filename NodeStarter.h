/*
 * ################################################################
 *
 * ProActive: The ProActive Linux Agent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 *
 * ################################################################
 */

/*
 * NodeStarter.h
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#ifndef NODESTARTER_H_
#define NODESTARTER_H_
//run  ccgnu2-config --help  for libraries and includes
#include <string>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include "Constants.h"
using namespace pa_agent_constants;
using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

//#include <cc++/thread.h>
//using namespace ost;
class NodeStarter {
public:
	NodeStarter(string name, string java_class, string security_policy,
			string log4j_file, string proactive_home, string, string java_bin);
	NodeStarter(const NodeStarter &node);
	virtual ~NodeStarter();
	virtual void run();
	int getPid();
	int RunCommand();
protected:
	string name;
	string java_class;
	string security_policy;
	string log4j_file;
	string proactive_home;
	string classpath;
	string java_bin;
private:
	log4cxx::LoggerPtr logger;
	int pid;

	void Initialize(string name, string java_class, string security_policy,
			string log4j_file, string proactive_home, string classpath,
			string java_bin);
};

#endif /* NODESTARTER_H_ */

