'''
Created on Jul 7, 2010

@author: cmathieu
'''
import io
import logging
import os
import subprocess
import time
 
logger = logging.getLogger("controller")

class Starter():
    def __init__(self, cmd, nice=0, ionice={'class': 3, 'classdata': None}, memory={'ram' : 4096, 'swap' : 0, 'swappiness' : 0}):
        self._cmd = cmd
        self._nice = nice
        self._ionice = ionice
        self._memory = memory
        
    def pinit(self):
        # CPU Nice
        os.nice(self._nice) # Can throw an OSError   
        
        # IONice
        ion_cmd  = ["/usr/bin/ionice", "-p %s" % os.getpid() , "-c %s" % self._ionice['class']]
        if self._ionice['classdata'] is not None:
            ion_cmd.append("-n" % self._ionice['classdata'])
        retcode = subprocess.call(ion_cmd)
        assert retcode == 0
        
        # Memory limit
        if self._memory is not None:
            mount_point = "/home/cmathieu/cg"
            group = "agent"
            p = os.path.join(mount_point, group)
            m = os.path.join(p, "memory.limit_in_bytes")
            with io.open(m, "w") as file:
                file.write(unicode(self._memory['ram']))
                    
            m = os.path.join(p, "memory.swappiness")
            with io.open(m, "w") as file:
                file.write(unicode(self._memory['swappiness']))
                
            m = os.path.join(p, "memory.memsw.limit_in_bytes")
            with io.open(m, "w") as file:
                file.write(unicode(self._memory['swap']))
            
    def start(self):
        self.p = subprocess.Popen(self._cmd, bufsize=4096, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, preexec_fn=self.pinit)
        
    def is_running(self):
        return self.p.poll() is None;
    
    def poll(self):
        return self.p.poll()
        
    def terminate(self):
        #TODO: This method is buggy. It should also clean the child tasks.
        self.p.terminate()

    def communication(self):
        self.p.communicate()

def __offset_to_timestamp(offset):
    return int(time.time()) + offset
      
def __wait_until_event(offset):
    time.sleep(offset)      
        
def mainloop(eventGenerator, speedup=1, bias=0):
 
    gen = eventGenerator.getActions()
    
    starter = None
    while True:
        try:
            (action, offset, func) = gen.next()
            timestamp = __offset_to_timestamp(offset)
            
             
            if action == "START":
                # Wait until the process must be started
                s = offset/speedup + bias
                logger.debug("next START in %s seconds" % s )
       
                time.sleep(s)      
                
                # Start the process
                logger.debug("STARTING epoch=%s cmd=%s " % (int(time.time()), func()))
                starter = Starter(func(), 10)
                starter.start()
                
                ofunc = func
                (action, offset, func) = gen.next()
                timestamp = __offset_to_timestamp(offset)
                
                if action == "STOP" or action == "RESTART":
                    logger.debug("next %s in %s seconds" % (action, offset))
                    # Monitor the process until the shutdown/restart
                    while time.time() < timestamp:
                        # TODO: Do something to redirect stdout/stderr into the loggers
                        
                        if starter.p.poll() is not None:
                            # The process died. Restart it
                            # TODO: We should clean the cgroup here
                            print "Child process died. Restart it"
                            starter = Starter(ofunc(), 10)
                            starter.start()

                        time.sleep(1)
                        
                        
                    if action == "STOP":
                        print "Killing child processes"
                        starter.terminate()
                        print "stdin:\n%s\nstdout\n%s\n" % starter.p.communicate()
                    elif action == "RESTART":
                        # TODO: Support restart and multiple restart 
                        assert False, "Restart not yet support: %s" % action
                    else: 
                        assert False, "Unspported action %s" % action
                    # Next event
                else:
                    assert False, "Unsupported action: %s" % action
            elif action == "STOP":
                assert starter is None, "Bad state, starter should be None"
                time.sleep(offset) # 
            elif action == "RESTART":
                assert False, "Bad restart. Restart must follow a start or restart"
            else:
                assert False, "Unsupported action: %s" % action
            
        except StopIteration:
            print "No more event..."
            return 
    
