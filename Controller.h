/*
 * Controller.h
 *
 *  Created on: Mar 19, 2009
 *      Author: vasile
 */

#ifndef GENERICS
#define GENERICS
using namespace std;
#include <signal.h>
#include <spawn.h>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <map>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <boost/interprocess/managed_shared_memory.hpp>



#endif

#ifndef CONTROLLER_H_
#define CONTROLLER_H_
const int TICK_DEFAULT=1000;
#define SHARED_SEGMENT_NAME 	"Shared_Memory_Segment"
#define PIDS_TICKS  			"pids_ticks"
#define PIDS_NODES  			"pids_nodes"
#define PIDS_WATCHERS  			"pids_watchers"
//ugly hack so I can associate pids with nodes
//could not get to work the shared maps with char*
//uses as a constant identifier the pid of the first
//node to be restarted
#define FIRST_PROCESS_WATCHER_PID		"old_pid_new_pid"


//boost
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <functional>
#include <utility>
//#include <algorithm>
using namespace boost::interprocess;

/*****************  DEFINING BOOST TYPES FOR SHARED MEMORY **************/
//Note that map<Key, MappedType>'s value_type is std::pair<const Key, MappedType>,
//so the allocator must allocate that pair.
typedef int  KeyType;
typedef int  MappedType;
typedef std::pair<const int, int> ValueType;

//Alias an STL compatible allocator of for the map.
//This allocator will allow to place containers
//in managed shared memory segments
typedef boost::interprocess::allocator<ValueType,
		boost::interprocess::managed_shared_memory::segment_manager>
		ShmemAllocator;
//Alias a map of ints that uses the previous STL-like allocator.
//Note that the third parameter argument is the ordering function
//of the map, just like with std::map, used to compare the keys.
typedef boost::interprocess::map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator> SharedMap;


typedef boost::interprocess::string  KeyTypeNodes;
typedef std::pair<const boost::interprocess::string, int> ValueTypeNodes;

//Alias an STL compatible allocator of for the map.
//This allocator will allow to place containers
//in managed shared memory segments
typedef boost::interprocess::allocator<ValueTypeNodes,
		boost::interprocess::managed_shared_memory::segment_manager>
		ShmemAllocatorNodes;
//Alias a map of ints that uses the previous STL-like allocator.
//Note that the third parameter argument is the ordering function
//of the map, just like with std::map, used to compare the keys.
typedef boost::interprocess::map<KeyTypeNodes, MappedType, std::less<KeyTypeNodes>,
	ShmemAllocatorNodes> SharedMapNodes;

/*******************  END BOOST TYPES FOR SHARED MEMORY ****************/


class Controller {
private:
	char* default_shell;
	char* default_node_exec;
//	static managed_shared_memory segment;
// uses the OLD_PIDS_NEW_PIDS shared map to
// reassociate when the Node is restarted
//uses as a constant identifier the pid of the first
//node to be started, on sub
//    SharedMap <int1,int2>   int1 the first pid for the node
//							   int2  the current pid of the node
	//int the first pid of the node
	//char* the name of the node
	std::map <std::string,int> nodes_pids;

public:
	Controller();
	virtual ~Controller();
	/**
	 * Starts a JVM with the default node starter and shell.
	 * shell - the full path to the Linux shell used to start
	 * 		the node executable
	 * node_executable - the node executable (bash script) used
	 * 		to start a JVM with the node
	 * 		usually:  PROACTIVE_FOLDER/bin/startNode.sh
	 * node_name - the name of the node to be started
	 *		can be in the form ibis://localdomain.localhost/node
	 *		in order to specify a protocol. See the ProActive manuals
	 *		for more information.
	 *
	 * Returns 0 if there is an error or the node_executable or
	 * the shell are not set.
	 * Returns the pid of the shell process which starts the Node
	 * on successful execution.
	 */
	int StartNode(char* shell, char* node_executable, char* node_name);
	/**
	 * Starts a JVM with the default node starter and shell.
	 * node_name - the name of the node to be started
	 *		can be in the form ibis://localdomain.localhost/node
	 *		in order to specify a protocol. See the ProActive manuals
	 *		for more information.
	 *
	 * Returns 0 if there is an error or the node_executable or
	 * the shell are not set.
	 * Returns the pid of the shell process which starts the Node
	 * on successful execution.
	 */
	int StartNode(char* node_name);
	/**
	 * Starts a watcher for the process with the give pid. The
	 * watcher keeps the process alive by checking it every
	 * milliseconds interval specified in the map contained in
	 * the shared memory segment. If the process stops it will
	 * be restarted with the passed shell, node_executable, and
	 * node_name. The watcher runs in it's own process, restarts
	 * the Node when the answer to kill -0 is -1, starts another
	 * watcher and then stops itself.
	 * //TODO - should not stop itself? any good reason for it
	 *
	 * shell - the full path to the Linux shell used to start
	 * 		the node executable
	 * node_executable - the node executable (bash script) used
	 * 		to start a JVM with the node
	 * 		usually:  PROACTIVE_FOLDER/bin/startNode.sh
	 * node_name - the name of the node to be started
	 *		can be in the form ibis://localdomain.localhost/node
	 *		in order to specify a protocol. See the ProActive manuals
	 *		for more information.
	 * pid - the pid of the process to watch
	 */
	void StartWatcher(char* shell, char* node_executable,
			char* node_name, int pid);
	/**
	 * Stops the watcher assigned to the corresponding pid.
	 * pid - the pid of the process being watched
	 *
	 * Returns false if no watcher has been found.
	 *
	 */
	bool StopWatcher(int pid);
	/**
	 * Stops the watcher assigned to the corresponding node_name.
	 * node_name - the Node name  being watched
	 *
	 * Returns false if no watcher has been found for the Node name.
	 *
	 */
	bool StopWatcher(char* node_name);

	/**
	 * Stops the watcher and the Node assigned to the corresponding pid.
	 * pid - the pid of the process being watched
	 *
	 *
	 * Returns false if no Node has been found.
	 */
	bool StopNode(int pid);

	/**
	 * Stops the Node with the give name.
	 * node_name - the name of the node to stop
	 *		Should be given in the full form
	 *		i.e. ibis://localdomain.localhost/node
	 *		i.e. node1
	 */
	void StopNode(char* node_name);

	/**
	 * Resets the time between checks to their default
	 * value TICK_DEFAULT.
	 */
	void ResetTicks();

	/**
	 * Sets the time between checks (ticks) for a
	 * particular process identified by pid.
	 *
	 * pid - the pid to set the tick for
	 * tick - the time between checks in milliseconds
	 */
	void SetTick(int pid, int tick);

	/**
	 * Gets the time between checks (ticks) for a
	 * particular process identified by pid.
	 *
	 * pid - the pid to set the tick for
	 * Returns the tick time in milliseconds.
	 */
	int GetTick(int pid);

	/**
	 * Sets the time between checks (ticks) for a
	 * particular Node identified by node_name.
	 *
	 * node_name - the name of the node to set the tick for
	 *		Should be given in the full form
	 *		i.e. ibis://localdomain.localhost/node
	 *		i.e. node1
	 * tick - the time between checks in milliseconds
	 */
	void SetTick(char* node_name, int tick);

	/**
	 * Gets the time between checks (ticks) for a
	 * particular Node identified by name.
	 *
	 * node_name - the name of the node to set the tick for
	 *		Should be given in the full form
	 *		i.e. ibis://localdomain.localhost/node
	 *		i.e. node1
	 * Returns the tick time in milliseconds.
	 */
	int GetTick(char* node_name);

	/**
	 * Sets the default shell path used for running
	 * the scripts which start the Nodes.
	 *
	 * shell - the shell with the full path i.e /usr/bin/bash
	 */

	void SetDefaultShell(char* shell);
	/**
	 * Gets the default shell path used for running
	 * the scripts which start the Nodes.
	 *
	 * Returns the shell executable path
	 */

	char* GetDefaultShell();

	/**
	 * Sets the default Node starter script  path used for
	 * starting Nodes.
	 *
	 * node_executable - the starNode script with
	 * 	the full path i.e PROACTIVE_FOLDER/bin/startNode.sh
	 */

	void SetDefaultNodeStarter(char* node_executable);

	/**
	 * Gets the default Node starter script  path used for
	 * starting Nodes.
	 *
	 * Returns the starNode script path
	 */
	char* GetDefaultNodeStarter();


	void SetWatcherPid(int process_pid, int watcher_pid);

	int GetWatcherPid(int process_pid);


	void AssociateNodesPids(int pid, std::string node);
	static void CreateMemorySegment();
	//TODO segfaults for some reason
	SharedMap* GetMemorySegment();
};

#endif /* CONTROLLER_H_ */
