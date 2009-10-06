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
/**
 * Base class for all Action classes.
 * */
class Action {
public:
	Action();
	virtual ~Action();
	/** Sets the time to wait in seconds
	 * before the JVM node for this action is
	 * restarted by the monitoring Watcher.
	 * @param delay delay in seconds befor the action is restarted
	 * */
	void SetRestartDelay(int delay);
	/**
	 * Sets the Java class used to start the JVM node
	 * for this action.
	 * @param starter class name used for starting this action
	 */
	void SetStarterClass(string starter);
	/** Sets the node name for the node started by this action
	 * @param name node name
	 * */
	void SetNodeName(string name);
	/**
	 * Modifies the enabled value for this action.
	 * This value is checked before attempting to start the
	 * JVM node corresponding to this action.
	 * @param enabled true if the action should be started, false otherwise
	 */
	void SetEnabled(bool enabled);
	/**
	 * @return the wait time in seconds before this action is restarted
	 */
	int GetRestartDelay() const;
	/**
	 * @return the class name used by this action to start a JVM node
	 */
	string GetStarterClass() const;
	/**
	 * @return the node name given to the node started by this action
	 */
	string GetNodeName() const;
	/**
	 * @return true if this action is enabled, false otherwise
	 */
	bool IsEnabled() const;
private:
	/** Delay in seconds before this action is restarted.*/
	int restart_delay;
	/** Java class used to start this action. */
	string starter_class;
	/** The name of the node started for this action*/
	string node_name;
	bool enabled;

};

#endif /* ACTION_H_ */
