/*
 * Controller.cpp
 *
 *  Created on: Mar 19, 2009
 *      Author: vasile
 */

#include "Controller.h"
//#include "SharedSegment.h"


Controller::Controller() {
	 using namespace boost::interprocess;
	   //Shared memory front-end that is able to construct objects
	   //associated with a c-string. Erase previous shared memory with the name
	   //to be used and create the memory segment at the specified address and initialize resources
	   cout << "Deleting shared memory " << endl;
	   shared_memory_object::remove(SHARED_SEGMENT_NAME);

	   try{
		   cout << "Creating shared segment " << endl;

	      managed_shared_memory segment
	         (create_only
	         ,SHARED_SEGMENT_NAME //segment name
	         ,65536);          //segment size in bytes

	      //Initialize the shared memory STL-compatible allocator
	      ShmemAllocator alloc_inst (segment.get_segment_manager());

	      ShmemAllocatorNodes alloc_inst_nodes (segment.get_segment_manager());

	      //Construct a shared memory map.
	      //Note that the first parameter is the comparison function,
	      //and the second one the allocator.
	      //This the same signature as std::map's constructor taking an allocator
		   cout << "Creating shared memory map " << endl;
	      SharedMap *pid_ticks =
	         segment.construct<SharedMap>(PIDS_TICKS)      //object name
	                                    (std::less<int>() //first  ctor parameter
	                                    ,alloc_inst);     //second ctor parameter
	      SharedMap *pid_watcher_pids =
	         segment.construct<SharedMap>(PIDS_WATCHERS)      //object name
	                                    (std::less<int>() //first  ctor parameter
	                                    ,alloc_inst);     //second ctor parameter
//	      SharedMapNodes *nodes_pids =
//	         segment.construct<SharedMapNodes>("cu_string")      //object name
//	                                    (std::less<boost::interprocess::string>() //first  ctor parameter
//	                                    ,alloc_inst_nodes);     //second ctor parameter


	      //ugly hack so I can associate pids with nodes
	  	//could not get to work the shared maps with char*
//
//	      SharedMap *old_pids_new_pids =
//	         segment.construct<SharedMap>(FIRST_PROCESS_WATCHER_PID)      //object name
//	                                    (std::less<int>() //first  ctor parameter
//	                                    ,alloc_inst);     //second ctor parameter

	   }
	   catch(...){
	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	      throw;
	   }
//	   shared_memory_object::remove(SHARED_SEGMENT_NAME);
	   cout << "Done creating shared memory" << endl;
}

Controller::~Controller() {
}

int Controller::StartNode(char *shell,char* node_executable, char* node_name ){
	pid_t pid, sid;

	/* already a daemon */
//	if ( getppid() == 1 ) return 0;

	/* Fork off the parent process */
	pid = fork();
	//If the fork was successful execute child code
	if (pid == 0) {
		/* At this point we are executing as the child process */
		/* Change the file mode mask */
		umask(0);
		/* Create a new SID for the child process */
		sid = setsid();
		cout << "Sid child " << sid << endl;
		//get the shotgun ready for zombies ?
//
//		if (sid < 0) {
//			exit(EXIT_FAILURE);
//		}

		/* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
		chdir("/");
		/* Redirect standard files to /dev/null */
//		freopen( "/dev/null", "r", stdin);
//		freopen( "/dev/null", "w", stdout);
//		freopen( "/dev/null", "w", stderr);

		int piexec =  execlp(shell, "-c", node_executable,
				node_name, (char *) 0);
		//if the node fails to start in the child process return -1
		//otherwise return the pid (=0)
		if (piexec == -1 ) return -1;
		else {
			cout << "Returning from children" << endl;
			return pid;
		}
	}
	//failed to fork
	else if (pid < 0)
	{
		cerr << "Failed to fork" << endl;
		return -1;
	}
	else {
		cout << "Returning from parent" << endl;
		//add to the list of pids and set a default tick
		SetTick(pid, TICK_DEFAULT);
		//add the node name
//		StartWatcher(shell, node_executable ,node_name, pid);
		cout << "Started watcher" << endl;
		return pid;
	}
}

int Controller::StartNode(char* node_name){
	return StartNode(default_shell, default_node_exec, node_name);
}

void Controller::StartWatcher(char* shell, char* node_executable,
	char* node_name, int jvm_pid){
	int watcher_pid = fork();
	bool stop = false;
	cout << "Watcher forked with pid :" << watcher_pid << endl;
		//run in the child process
		if (watcher_pid == 0 ){
			umask(0);
			cout << "Checking "<< jvm_pid << " with an interval of "
				<< GetTick(jvm_pid) << endl;
			while (!stop){
				//usleep takes microseconds, we use milliseconds
				cout << "Checking "<< jvm_pid << " with an interval of "
							<< GetTick(jvm_pid) << endl;
				usleep(GetTick(jvm_pid)*1000);
				// if a signal cannot be sent - return is -1
				// start the jvm again
				if (kill(jvm_pid, 0) == -1 ) {
					cout << "Node " <<  jvm_pid  << "has been killed" << endl;
					cout << "Signaling the controller and stopping thread..." << endl;
					stop = true;
					//restarting JVM
					StartWatcher(shell, node_executable ,node_name,
							StartNode(shell, node_executable, node_name));

					exit(0);
				}
			}
		}
		else{ cout << "In else" <<endl;}
		//sets watcher pid in the parent process
		SetWatcherPid(jvm_pid, watcher_pid);


}


void Controller::SetTick(int pid, int tick){
	cout << "Changing tick to " << tick << " for pid " << pid << endl;
	using namespace boost::interprocess;
	   try{
	      //A special shared memory where we can
	      //construct objects associated with a name.
	      //Connect to the already created shared memory segment
	      //and initialize needed resources
	      managed_shared_memory segment
	         (open_only
	         ,SHARED_SEGMENT_NAME);  //segment name
	      //Find the vector using the c-string name
	      SharedMap *mappp = segment.find<SharedMap>(PIDS_TICKS).first;
	      //delete old value by key
	      mappp->erase(pid);
	      //Insert data in the map
	      mappp->insert(std::pair<const int, int>(pid, tick));
	   }
	   catch(...){
		   cout << "Catched";
	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	      throw;
	   }
//	SharedMap *shared_map = GetMemorySegment();
//	shared_map->erase(pid);
//	shared_map->insert(std::pair<const int, int>(pid, tick));
}

void Controller::SetWatcherPid(int process_pid, int watcher_pid){
	using namespace boost::interprocess;
	   try{
		   //attach memory segment
	      SharedMap::iterator it;
		   managed_shared_memory segment
	         (open_only
	         ,SHARED_SEGMENT_NAME);  //segment name
	      //Find the vector using the c-string name
	      SharedMap *pids_watchers = segment.find<SharedMap>(PIDS_WATCHERS).first;
	      //first_process, old_watcher
	      SharedMap *first_process_watcher = segment.find<SharedMap>(PIDS_WATCHERS).first;



	      //old_process, old_watcher
	      //get old_watcher
	      it = pids_watchers->find(process_pid);

	      //first_process, old_watcher
	      //get first_process by old_watcher
	   //   it = first_process_watcher->find((*it).second);

	      //erase old pair
	      cout << "First process pid:  "<<(*it).first << endl;

	      cout << "Old Watcher pid:  "<<(*it).second <<endl;
	      first_process_watcher->erase((*it).second);

	      //first_process, new_watcher
	      //set new_watcher
	      first_process_watcher->insert(std::pair<const int, int>((*it).first, watcher_pid));


	      //delete old value by key
	      pids_watchers->erase(process_pid);
	      //Insert data in the map
	      pids_watchers->insert(std::pair<const int, int>(process_pid, watcher_pid));

	   }
	   catch(...){
		   cout << "Catched";
	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	      throw;
	   }

}



int Controller::GetWatcherPid(int process_pid){

	using namespace boost::interprocess;
	   try{
		   SharedMap::iterator it;
		   //attach memory segment
		   managed_shared_memory segment
	         (open_only
	         ,SHARED_SEGMENT_NAME);  //segment name
	      //Find the vector using the c-string name
	      SharedMap *pids_watchers = segment.find<SharedMap>(PIDS_WATCHERS).first;
	      //returning the tick;
		it = pids_watchers->find(process_pid);
	    return (*it).second;
	   }
	   catch(...){
		   cout << "Catched";
	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	      throw;
	   }
}

int Controller::GetTick(int pid){
	 using namespace boost::interprocess;
	 try{
		   SharedMap::iterator it;
	      //A special shared memory where we can
	      //construct objects associated with a name.
	      //Connect to the already created shared memory segment
	      //and initialize needed resources
	      managed_shared_memory segment
	         (open_only
	         ,SHARED_SEGMENT_NAME);  //segment name
	      //Find the vector using the c-string name
	      SharedMap *mappp = segment.find<SharedMap>(PIDS_TICKS).first;
	      //returning the tick;
		it = mappp->find(pid);
	    return (*it).second;

	   }
	   catch(...){
	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	      throw;
	   }
	  cerr << "Shit happened in GetTick."<< endl;
	  return 0;
}

bool Controller::StopWatcher(int pid){
	//send a kill signal to the watcher process
	 using namespace boost::interprocess;
	 try{
		   SharedMap::iterator it;
		  //attach shared memory segment
	      managed_shared_memory segment
	         (open_only
	         ,SHARED_SEGMENT_NAME);  //segment name
	      //Find the vector using the c-string name
	      SharedMap *pids_watchers = segment.find<SharedMap>(PIDS_WATCHERS).first;
	      SharedMap *pid_ticks = segment.find<SharedMap>(PIDS_TICKS).first;

	      //returning the tick;
		it = pids_watchers->find(pid);
	    int watcher_pid = (*it).second;
	    //terminate the watcher process if the watcher pid is found
	    if (it != pids_watchers->end()) {
	    	kill(watcher_pid, 15);
	    	//remove references from shared memory
	    	pids_watchers->erase(pid);
	    	pid_ticks->erase(pid);
	    	return true;
	    }
	    else{
	    	//return false if the pid was not found
	    	return false;
	    }
	   }
	   catch(...){
	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	      throw;
	   }
	  cerr << "Error"<< endl;
	  return -1;
}

bool Controller::StopWatcher(char *node_name){
	std::map<std::string,int>::iterator it;
	// return the first pid of the Node (see explanation in map declaration)

		it = nodes_pids.find(node_name);
	    //if a pid is found get the current pid for the watcher and terminate
	    if (it != nodes_pids.end()) {
	    	int first_pid = (*it).second;
	    	cout << "Found first pid associated to Node name:" << first_pid <<endl;
	   	 using namespace boost::interprocess;
	   	 try{
	   		   SharedMap::iterator current_pid_it;
	   	      managed_shared_memory segment
	   	         (open_only
	   	         ,SHARED_SEGMENT_NAME);  //segment name
	   	      //Find the vector using the c-string name
	   	      SharedMap *old_new_pids = segment.find<SharedMap>(FIRST_PROCESS_WATCHER_PID).first;
	   	      //returning the tick;
	   	   current_pid_it = old_new_pids->find(first_pid);
	   	    int watcher_pid = (*current_pid_it).second;
		   	cout << "Found current_pid associated to Node name:" << watcher_pid <<endl;

	   	 return StopWatcher(watcher_pid);
	   	   }
	   	   catch(...){
	   	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	   	      throw;
	   	   }
	    }
	    else{
	    	//return false if the node_name was not found
	    	return false;
	    }
}


void Controller::ResetTicks() {

}


void Controller::SetDefaultShell(char* shell){
	default_shell = shell;
}
char* Controller::GetDefaultShell(){
	return default_shell;
}
void Controller::SetDefaultNodeStarter(char* node_executable){
	default_node_exec = node_executable;
}
char* Controller::GetDefaultNodeStarter(){
	return default_node_exec;
}



void Controller::AssociateNodesPids(int pid, std::string node){
	nodes_pids.insert(std::pair<std::string,int>(node,pid));
}




/*************************/

//TODO check why it segfaults
SharedMap* Controller::GetMemorySegment(){
	using namespace boost::interprocess;
	   try{
	      //A special shared memory where we can
	      //construct objects associated with a name.
	      //Connect to the already created shared memory segment
	      //and initialize needed resources
	      managed_shared_memory segment
	         (open_only
	         ,SHARED_SEGMENT_NAME);  //segment name
	      //Find the vector using the c-string name
	      SharedMap *mappp = segment.find<SharedMap>(SHARED_SEGMENT_NAME).first;
	      return mappp;
	   }
	   catch(...){
		   cout << "Catched";
	      shared_memory_object::remove(SHARED_SEGMENT_NAME);
	      throw;
	   }

}

