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

class ConfigParserTest: public ::testing::Test {
public:
	ConfigParserTest();
	virtual ~ConfigParserTest();
protected:
	virtual void SetUp();
	virtual void TearDown();
};

#endif /* CONFIGParserTEST_H_ */
