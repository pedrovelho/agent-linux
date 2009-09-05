/*
 * P2PAction.h
 *
 *  Created on: Sep 5, 2009
 *      Author: vasile
 */

#ifndef P2PACTION_H_
#define P2PACTION_H_

#include "Action.h"

class P2PAction: public Action {
public:
	P2PAction();
	virtual ~P2PAction();

	void SetContact(string contact);
	void SetProtocol(string protocol);
	string GetContact() const;
	string GetProtocol() const;
private:
	string contact;
	string protocol;
};

#endif /* P2PACTION_H_ */
