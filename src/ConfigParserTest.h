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
 * ConfigParserTest.h
 *
 *  Created on: Sep 7, 2009
 *      Author: vasile
 */

#ifndef CONFIGPARSERTEST_H_
#define CONFIGPARSERTEST_H_

#include <gtest/gtest.h>
#include "ConfigParser.h"
using namespace paagent;
class ConfigParserTest: public ::testing::Test {
public:
	ConfigParserTest();
	virtual ~ConfigParserTest();
protected:
	virtual void SetUp();
	virtual void TearDown();
	Configuration *config;
};

#endif /* CONFIGParserTEST_H_ */
