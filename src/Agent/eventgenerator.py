#! /usr/bin/env python
# -*- coding: UTF-8 -*-
#################################################################
#
# ProActive Parallel Suite(TM): The Java(TM) library for
#    Parallel, Distributed, Multi-Core Computing for
#     Enterprise Grids & Clouds
#
# Copyright (C) 1997-2010 INRIA/University of 
#                 Nice-Sophia Antipolis/ActiveEon
# Contact: proactive@ow2.org or contact@activeeon.com
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; version 3 of
# the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
# USA
#
# If needed, contact us to obtain a release under GPL Version 2 
# or a different license than the GPL.
#
#  Initial developer(s):               The ProActive Team
#                        http://proactive.inria.fr/team_members.htm
#  Contributor(s):
#
#################################################################
# $$ACTIVEEON_INITIAL_DEV$$
#################################################################

from main import AgentError, AgentInternalError
import logging
import main
import time
import os
import subprocess
import io
import threading 
import time
import utils

days = {
        "monday": 0,
        "tuesday" : 1,
        "wednesday" : 2,
        "thursday" : 3,
        "friday" : 4,
        "saturday" : 5,
        "sunday" : 6
}

_ONE_WEEK_IN_SECS = (60*60*24*7)

def _seconds_elespased_since_start_of_week():
    ''' 
    Returns the number of seconds elapsed since the beginning of the week (Monday 00:00)
    '''    
    now =  time.localtime()
    return now[5] + (now[4]*60) + (now[3]*60*60) + (now[6]*60*60*24)

def _start_of_week_epoch():
    '''
    Returns the epoch time of the start of the week
    '''
    current_epoch = int(time.time())
    return current_epoch - _seconds_elespased_since_start_of_week()



    

class EventConfig(object):
    '''
    The configuration associated to an event
    
    This configuration is used by the action to fork the JVM with the right options
    '''            
    def __init__(self):
        self.proactiveHome = None
        self.javaHome = None
        self.jvmParameters = []
        self.memoryLimit = 0
        self.nbRuntimes = utils.get_number_of_cpus()
        self.protocol = "rmi"
        self.portRange = None
        self.onRuntimeExitScript = None
        self.nice = 0
        self.ionice = None
        
    def check(self):
        '''
        Check the state of this event is consistent 
        '''
        def raiseError(message):
            raise AgentError("Invalid event configuration: " + message)
        
        if self.proactiveHome is None:
            raiseError("ProActive home is not set")
        if len(self.proactiveHome) < 1:
            raiseError("ProActive home cannot be empty") # FIXME: XSD should disallow this 
        
        if self.javaHome is None:
            raiseError("Java home is not set")
        if len(self.javaHome) < 1:
            raiseError("Java home cannot be empty") # FIXME: XSD should disallow this
        
        if self.memoryLimit < 0:
            raiseError("Memory limit must be a positive integer")
        
        if self.nbRuntimes < 0:
            raiseError("The number of runtimes must be a positive integer")
        
        if self.protocol is None:
            raiseError("ProActive communication protocol is not set")
        if len(self.protocol) < 1:
            raiseError("ProActive communication protocol cannot be empty") # FIXME: XSD should disallow this
            raise AgentError("FIXME")
        
        if self.portRange is not None:
            if self.portRange[0] < 0 or self.portRange[0] > 65536:
                raiseError("First TCP port must be an integer between 0 and 65536")
            if self.portRange[1] < 0 or self.portRange[1] > 65536:
                raiseError("Last TCP port must be an integer between 0 and 65536")
            if self.portRange[0] > self.portRange[1]:
                raiseError("Last TCP port must be greater or equals to the first TCP port")
            if self.portRange[1] - self.portRange[0] < self.nbRuntimes:
                raiseError("The port range is too small according to the number of runtimes")
                
        if self.nice < -20 or self.nice > 19:
            raiseError("Invalid nice value. Must be betweeon -20 and 19")
            
        if self.ionice is not None:
            if self.ionice["class"] == 1 or self.ionice["class"] == 2:
                if self.ionice["data"] is None:
                    raiseError("A class data is mandatory with the best effort and real time ionice classes")
            else:
                if self.ionice["data"] is not None:
                    raiseError("none and idle ionice classes does not accept any class data argument")
            
    def parse(self, eventNode):
        '''
        Fill this config with the value extracted from XML configuration file
        '''
        confNode = eventNode.xpath("/a:agent/a:config", namespaces = {'a' : main.xmlns})
        assert(len(confNode) == 1)
        self.__parse_config(True, confNode[0])

        eventNode = eventNode.xpath("./a:config", namespaces = {'a' : main.xmlns})
        if len(eventNode) == 1:
            self.__parse_config(False, eventNode[0])

    def __parse_config(self, isRoot, confNode):
        lx = confNode.xpath("./a:proactiveHome", namespaces = {'a' : main.xmlns})
        if isRoot: # This element is mandatory in the root config but optional otherwise
            assert(len(lx) == 1)
            self.proactiveHome = lx[0].text
        else:
            assert(len(lx) == 1 or len(lx) == 0)
            if len(lx) == 1:
                self.proactiveHome = lx[0].text
                
        lx = confNode.xpath("./a:javaHome", namespaces = {'a' : main.xmlns})
        if isRoot: # This element is mandatory in the root config but optional otherwise
            assert(len(lx) == 1)
            self.javaHome = lx[0].text
        else:
            assert(len(lx) == 1 or len(lx) == 0)
            if len(lx) == 1:
                self.javaHome = lx[0].text

        lx = confNode.xpath("./a:jvmParameters/a:param", namespaces = {'a' : main.xmlns})
        if len(lx) == 0 and not isRoot:
            self.jvmParameters = [] 
        for node in lx:
            self.jvmParameters.append(node.text)

        lx = confNode.xpath("./a:memoryLimit", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1 or len(lx) == 0)
        if len(lx) == 1:
            self.memoryLimit = int(lx[0].text)
        
        lx = confNode.xpath("./a:nbRuntimes", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1 or len(lx) == 0)
        if len(lx) == 1: 
            self.nbRuntimes = int(lx[0].text)
            if self.nbRuntimes == "auto":
                self.nbRuntimes = 0

        lx = confNode.xpath("./a:protocol", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1 or len(lx) == 0)
        if len(lx) == 1: 
            self.protocol = lx[0].text

        lx = confNode.xpath("./a:portRange", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1 or len(lx) == 0)
        if len(lx) == 1: 
            self.portRange = (int(lx[0].get("first")), int(lx[0].get("last")))

        lx = confNode.xpath("./a:onRuntimeExitScript", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1 or len(lx) == 0)
        if len(lx) == 1: 
            self.onRuntimeExitScript = lx[0].text

        lx = confNode.xpath("./a:nice", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1 or len(lx) == 0)
        if len(lx) == 1: 
            self.nice = int(lx[0].text)
            
        lx = confNode.xpath("./a:ionice", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1 or len(lx) == 0)
        if len(lx) == 1:
            classes = {'none' : 0, 'realtime' : 1, 'besteffort' : 2, 'idle' : 3}
            clazz = classes[lx[0].get("class")]
            data = lx[0].get("data")
            self.ionice = {"class" : clazz, "data" : data}
            

class AgentTime(object):
    
    def __init__(self):
        # Origin is the set to the start of the current week
        now = int(time.time())
        tm = time.localtime()
        offset = tm[5] + (tm[4]*60) + (tm[3]*60*60) + (tm[6]*60*60*24)
        self.origin = now - offset

    def event_date_in_agent_time(self, weeks, offset_in_week):
        atime = weeks * _ONE_WEEK_IN_SECS
        atime+= offset_in_week
        return atime
    
    def event_date_in_epoch(self, weeks, offset_in_week):
        return self.origin + self.event_date_in_agent_time(weeks, offset_in_week)

    def epoch_to_agent_time(self, epoch):
        return epoch - self.origin

    def agent_time_to_epoch(self, atime):
        return atime + self.origin            
            
            
class SpecificEvent(object):
    def __init__(self, generic_event, epoch_date, type):
        self.config     = generic_event.config
        self.action     = generic_event.action
        self.type = type
        self.epoch_date = epoch_date
        self.duration   = generic_event.duration
        self.startOffset= generic_event.startOffset
        
    def wait_epoch_date(self):
        '''
        Wait until the start time of this event
        '''
        time.sleep(self.seconds_remaining())

    def seconds_remaining(self):
        '''
        Return the number of seconds remaining until the event
        '''
        return self.epoch_date - int(time.time())

class JVMStarter():
    '''
    This class is in  charge of spawning a new process for the JVM.
    '''
    def __init__(self, config, cmd, epoch_date):
        self.config = config
        self.cmd = cmd
        self.epoch_date = epoch_date
        self.is_canceled = False
      
    def schedule(self):
        sleep_time = int(self.epoch_date - time.time())
        print "JVM Starter thread created for %s. Executing will begin in %s secs" % (self.cmd, sleep_time) 
        time.sleep(sleep_time)
        
        self.p = None
        while not self.is_canceled:
            # Do we need to restart the process ? 
            if self.p is None or self.p.poll() is not None:
                if self.p is not None:
                    print "Process %s exited with status code %s" % (self.p.pid, self.p.poll())
                    (stdout, stderr) = self.p.communicate()
                    print stdout
                    print stderr
                
                self.p = subprocess.Popen(self.cmd, bufsize=4096, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, preexec_fn=self._pinit)
                print "Forked process with pid:%s cmd:%s" % (self.p.pid, self.cmd)
            time.sleep(1)
    
        self.p.terminate()
        (stdout, stderr) = self.p.communicate()
        print stdout
        print stderr        
        print "JVM Stater: cancelled"
        
    def cancel(self):
        self.is_canceled = True
        
    def _pinit(self):
        # CPU Nice
        if self.config.nice != 0:
            os.nice(self.action.nice) # Can throw an OSError   
        
        # IONice
        if self.config.ionice is not None:
            io_class = self.config.ionice['class']
            ion_cmd  = ["/usr/bin/ionice", "-p %s" % os.getpid() , "-c %s" % io_class]
            if self.config.ionice['classdata'] is not None:
                ion_cmd.append("-n" % self.config.ionice['classdata'])
            retcode = subprocess.call(ion_cmd)
            assert retcode == 0
        
        # Memory limit
        if self.config.memoryLimit > 0:
            mount_point = "/home/cmathieu/cg"
            group = "agent"
            p = os.path.join(mount_point, group)
            m = os.path.join(p, "memory.limit_in_bytes")
            with io.open(m, "w") as file:
                file.write(unicode(self.config.memoryLimit))
                    
            m = os.path.join(p, "memory.swappiness")
            with io.open(m, "w") as file:
                file.write(unicode(0))
                
            m = os.path.join(p, "memory.memsw.limit_in_bytes")
            with io.open(m, "w") as file:
                file.write(unicode(self.config.memoryLimit))
            
    
class StartEvent(SpecificEvent):
   
    def __init__(self, generic_event, epoch_date):
        super(StartEvent, self).__init__(generic_event, epoch_date, "START")
        self.forks = []
   
    def schedule(self):

        for i in xrange(self.config.nbRuntimes):
            cmd = self._build_java_cmd(i)
            starter = JVMStarter(self.config, cmd, self.epoch_date)
            thread = threading.Thread(None, starter.schedule, "THREAD%s" % i, (), {})
            thread.start()
            self.forks.append((starter, thread))
    
    def cancel(self):
        for (starter, thread) in self.forks:
            starter.cancel() 

        for (starter, thread) in self.forks:
            thread.join()

    def _build_java_cmd(self, id):
        cmd = []
        
        # Java
        cmd.append(self.config.javaHome + "/bin/java")
        # Classpath
        cmd.append("-classpath")
        cmd.append(self.config.proactiveHome +  "/dist/lib/*")
        for param in self.config.jvmParameters:
            cmd.append(param)
            
        cmd.append("-Dproactive.communication.protocol=%s" % self.config.protocol)

        if self.config.portRange is not None:
            port = self.config.portRange[0] + id
            assert port <= self.config.portRange[1] + id, "port range is to small according to nbRuntimes"
            
            if self.config.protocol == "rmi":
                cmd.append("-Dproactive.rmi.port=%d" % port)
            elif self.config.protocol == "http":
                cmd.append("-Dproactive.http.port=%d" % port)
            elif self.config.protocol == "pamr":
                pass # PAMR does not user server socket
            elif self.config.protocol == "pnp":
                cmd.append("-Dproactive.pnp.port=%d" % port)
            else:
                print "Port range is not supported for protocol: %s" % self.config.protocol

        cmd.append(self.action.getClass())
        map(cmd.append, self.action.getArguments())
        return cmd

    
class StopEvent(SpecificEvent): 
 
    def __init__(self, generic_event, epoch_date):
        super(StopEvent, self).__init__(generic_event, epoch_date, "STOP")
        self.duration = 0
        
    def schedule(self):
        pass
    
    def cancel(self):
        pass

    
class RestartEvent(SpecificEvent): 
    # FIXME: SUPPORT RESTART
    def __init__(self, old_event, new_event, epoch_date):
        super(RestartEvent, self).__init__(new_event, epoch_date, "RESTART")
        
    def schedule(self):
        pass

    def cancel(self):
        pass


class Event(object):
    '''
    
    '''
    def __init__(self, startOffset, duration, config, action):
        self.startOffset = startOffset
        self.duration = duration
        self.config = config
        self.action = action
    
    @property
    def stopOffset(self):
        stopOffset =  self.startOffset + self.duration
        if stopOffset > _ONE_WEEK_IN_SECS:
            stopOffset -= _ONE_WEEK_IN_SECS
        return stopOffset
    
    def check(self):
        '''
        Check this event is in a coherent state
        '''
        def raiseError(message, internal=False):
            if internal:
                raise AgentInternalError("Invalid event state")
            else:
                raise AgentError("Invalid event state: " + message)
  
        if self.startOffset < 0:
            raiseError("Start offset must be positive", True)
        if self.startOffset > _ONE_WEEK_IN_SECS:
            raiseError("Start offset must be lesser than %s" % _ONE_WEEK_IN_SECS, True)
        if self.duration < 1:
            raiseError("An event must last a least one second")
        if self.duration >= _ONE_WEEK_IN_SECS:
            raiseError("An event cannot last more than 7 days", True)
            raise AgentError("FIXME")
        
        self.config.check()
        
    def __str__(self):
        return "startOffset: %s, duration: %s, config: %s" % (self.startOffset, self.duration, self.config)   
    
     
     
     
class CalendarEventGenerator(object):
    '''
    The event generator. Each time the generator is invoked the next event to be executed
    is returned.
    '''
    
    def __init__(self, action=None):
        self.events = []
        self.action = action

    def __str__(self):
        return "%s" % self.events[0]
    
    def __get_start_offset(self, eventNode):
        ''' Return the start offset of this event in seconds'''
        lx = eventNode.xpath("./a:start", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1)

        startOffset = 0
        startOffset += int(lx[0].get("second"))
        startOffset += int(lx[0].get("minute")) * 60
        startOffset += int(lx[0].get("hour"))   * 60 * 60
        startOffset += days[lx[0].get("day")]   * 60 * 60 * 24
        return startOffset

    def __getDuration(self, eventNode): 
        ''' Return the duration of this event in seconds '''
        lx = eventNode.xpath("./a:duration", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1)

        duration = 0
        duration += int(lx[0].get("seconds"))
        duration += int(lx[0].get("minutes")) * 60
        duration += int(lx[0].get("hours"))   * 60 * 60
        duration += int(lx[0].get("days"))    * 60 * 60 * 24
        return duration
    
    def parse(self, tree):
        '''
        Parse all the events in the configuration file
        '''    
        lx = tree.xpath("/a:agent/a:events/a:event", namespaces = {'a' : main.xmlns})
        for eventNode in lx:
            config = EventConfig()
            config.parse(eventNode)
            startOffset = self.__get_start_offset(eventNode)
            duration    = self.__getDuration(eventNode)
                
            event = Event(startOffset, duration, config, self.action)
            self.events.append(event)
            
        # Sort events by start date    
        self.events.sort(key=lambda event: event.startOffset) 
            
        
    def check(self):
        '''
        Check that all the events are in a coherent state
        '''
        for event in self.events:
            event.check()

        self._checkOverlapping()
    
    def _checkOverlapping(self):
        ''' Check if two events overlap'''
        if len(self.events) > 1:
            for i in range(len(self.events)-1):
                if self.events[i].stopOffset > self.events[i+1].startOffset:
                    raise AgentError("Calendar events %s and %s overlap" % (self.events[i], self.events[i+1]))

            if (self.events[-1].stopOffset <= self.events[-1].startOffset) and (self.events[-1].stopOffset > self.events[0].startOffset):
                raise AgentError("Calendar events %s and %s overlap" % (self.events[-1], self.events[0]))
      
    def getActions(self, timebias=0):
        g = self.__getActions(timebias) 

        (nEvent, nIndex) = g.next()
        while True:
            (cEvent, cIndex) = (nEvent, nIndex)
            (nEvent, nIndex) = g.next()
            
            if nEvent.epoch_date == cEvent.epoch_date and cEvent.type == "STOP" and nEvent.type == "START": 
                # Converts current and next action into a restart
                yield (RestartEvent(cEvent, nEvent, nEvent.epoch_date))
                (cEvent, cIndex) = (nEvent, nIndex)
                (nEvent, nIndex) = g.next()
            else:
                yield (cEvent)
            
    def __getActions(self, timebias=0): 
       
        def offset_in_origin_week(agent_time):
            now = int(time.time())
            return now - agent_time.origin

        if len(self.events) == 0:
            raise StopIteration("No calendars event")

        agent_time = AgentTime()

        index = None
        wasStart = None
        weeks = 0
        # Tries to find the first event according to the current time offset since the start of the week
        for i in range(len(self.events)):
            if self.events[i].startOffset >= offset_in_origin_week(agent_time):
                index = i ; wasStart = True
                break
            if self.events[i].stopOffset >=  offset_in_origin_week(agent_time):
                index = i ; wasStart = False
                break

        mustResetWeeks = False
        if index is None: # No event until next week
            weeks +=1
            if self.events[0].startOffset < self.events[-1].stopOffset: 
                index = 0; wasStart = True
            else:
                index = len(self.events) - 1; wasStart = False
                mustResetWeeks = True # Ugly  hack, to avoid double weeks increment
                
        # Everything is set. It's time to yield the first event 
        if wasStart:
            yield (StartEvent(self.events[index], agent_time.event_date_in_epoch(weeks, self.events[index].startOffset)), index)
        else:
            yield (StopEvent(self.events[index], agent_time.event_date_in_epoch(weeks, self.events[index].stopOffset)), index)
        
        if mustResetWeeks: 
            weeks = 0
            
        # Subsequent calls
        # Since no overlapping is allowed we just have to loop over the events
        while True:  
            if wasStart:
                # Return the stop offset of the current event
                if self.events[index].stopOffset > self.events[index].startOffset:
                    yield (StopEvent(self.events[index], agent_time.event_date_in_epoch(weeks, self.events[index].stopOffset)), index)
                else: # stop is next week
                    yield (StopEvent(self.events[index], agent_time.event_date_in_epoch(weeks, self.events[index].stopOffset + _ONE_WEEK_IN_SECS)), index)
                wasStart = False
            else:
                # Go to the next event
                index += 1
                if index == len(self.events): # End of the list reached, hello new week !
                    index = 0
                    weeks +=1
                
                yield (StartEvent(self.events[index], agent_time.event_date_in_epoch(weeks, self.events[index].startOffset)), index)
                wasStart = True


def parse(tree, action):
    '''
    Parse all the calendar events in the configuration and return a CalendarEventGenerator
    '''
    evg = CalendarEventGenerator(action)
    evg.parse(tree)
    return evg
