/*
 * CustomAction.h
 *
 *  Created on: Sep 5, 2009
 *      Author: vasile
 */

#ifndef CUSTOMACTION_H_
#define CUSTOMACTION_H_

#include "Action.h"

class CustomAction: public Action {
public:
	CustomAction();
	virtual ~CustomAction();
	void SetArguments(string args);
	string GetArguments() const;
private:
	string arguments;
};

#endif /* CUSTOMACTION_H_ */
