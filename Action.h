/*
 * Action.h
 *
 *  Created on: Sep 5, 2009
 *      Author: vasile
 */

#ifndef ACTION_H_
#define ACTION_H_
#include <string>
#include <typeinfo>
using namespace std;
class Action {
public:
	Action();
	virtual ~Action();
	void SetRestartDelay(int delay);
	void SetStarterClass(string starter);
	void SetNodeName(string name);
	void SetEnabled(bool enabled);

	int GetRestartDelay() const;
	string GetStarterClass() const;
	string GetNodeName() const;
	bool GetEnabled() const;
private:
	int restart_delay;
	string starter_class;
	string node_name;
	bool enabled;

};

#endif /* ACTION_H_ */
