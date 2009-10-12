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
#include <string>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include "Configuration.h"
#include "Constants.h"
using namespace paagent::constants;
using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

//#include <cc++/thread.h>
//using namespace ost;
namespace paagent{
class NodeStarter {
public:
	NodeStarter(string name, string java_class);
	virtual ~NodeStarter();
	void run();
	int getPid();
	int RunCommand();
protected:
	vector <string> exec_arguments;
	string java_bin;
	string java_class;
	string name;
private:
	log4cxx::LoggerPtr logger;
	int pid;
	void Initialize();
};
} //namespace paagent
#endif /* NODESTARTER_H_ */

