/*
 * RMAction.h
 *
 *  Created on: Sep 5, 2009
 *      Author: vasile
 */

#ifndef RMACTION_H_
#define RMACTION_H_

#include "Action.h"

class RMAction: public Action {
public:
	RMAction();
	virtual ~RMAction();
	void SetUsername(string user);
	void SetPassword(string pass);
	void SetURL(string url);

	string GetUsername() const;
	string GetPassword() const;
	string GetURL() const;

private:
	string username;
	string password;
	string url;
};

#endif /* RMACTION_H_ */
