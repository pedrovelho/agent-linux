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

import logging
import main
import time
from main import AgentError, AgentInternalError

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

def secondsSinceStartOfWeeK():
    ''' 
    Returns the number of seconds elapsed since the beginning of the week (Monday 00:00)
    '''    
    now =  time.localtime()
    return now[5] + (now[4]*60) + (now[3]*60*60) + (now[6]*60*60*24)


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
        self.nbRuntimes = 0
        self.protocol = "rmi"
        self.portRange = (0, 0)
        self.onRuntimeExitScript = None
      
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
        
        if self.portRange[0] < 0 or self.portRange[0] > 65536:
            raiseError("First TCP port must be an integer between 0 and 65536")
        if self.portRange[1] < 0 or self.portRange[1] > 65536:
            raiseError("Last TCP port must be an integer between 0 and 65536")
        if self.portRange[0] > self.portRange[1]:
            raiseError("Last TCP port must be greater or equals to the first TCP port")
            
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
        if len(lx) != 0 and not isRoot:
            self.jvmParameters = [] 
        for node in lx:
            self.jvmParameters.append(node.text)

        lx = confNode.xpath("./a:memoryManagement", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1 or len(lx) == 0)
        if len(lx) == 1:
            isEnabled = lx[0].get("enabled", "false")
            if isEnabled == "true" or isEnabled == 0:
                self.memoryLimit = int(lx[0].text)
            else:
                self.memoryLimit = 0
        
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

        
    
 

class Event(object):
    '''
    
    '''
    def __init__(self, startOffset, duration, config):
        self.startOffset = startOffset
        self.duration = duration
        self.config = config
    
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
            if duration == 0:
                duration = 1
                
            event = Event(startOffset, duration, config)
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

        (nAction, nTimestamp, nFunc, nIndex) = g.next()
        while True:
            (cAction, cTimestamp, cFunc, cIndex) = (nAction, nTimestamp, nFunc, nIndex)
            (nAction, nTimestamp, nFunc, cIndex) = g.next()
            
            if nTimestamp == cTimestamp and cAction == "STOP" and nAction == "START":
                # Converts current and next action into a restart
                yield ("RESTART", cTimestamp, self.action.getRestart(self.events[nIndex].config))
                (cAction, cTimestamp, cFunc, cIndex) = (nAction, nTimestamp, nFunc, nIndex)
                (nAction, nTimestamp, nFunc, nIndex) = g.next()
            else:
                yield (cAction, cTimestamp, cFunc)
            
    def __getActions(self, timebias=0): 
        def currentOffset():
            return secondsSinceStartOfWeeK() + timebias
               
        def remaining(offset):
            ret = offset - currentOffset() + (weeks*_ONE_WEEK_IN_SECS)
            assert ret >= 0
            return ret 

        if len(self.events) == 0:
            raise StopIteration("No calendars event")

        index = None
        wasStart = None
        weeks = 0
        # Tries to find the first event according to the current time offset since the start of the week
        for i in range(len(self.events)):
            if self.events[i].startOffset >= currentOffset():
                index = i ; wasStart = True
                break
            if self.events[i].stopOffset >= currentOffset():
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
            yield ("START", remaining(self.events[index].startOffset), self.action.getStart(self.events[index].config), index)
        else:
            yield ("STOP", remaining(self.events[index].stopOffset), None, index)
        
        if mustResetWeeks: 
            weeks = 0
            
        # Subsequent calls
        # Since no overlapping is allowed we just have to loop over the events
        while True:  
            if wasStart:
                # Return the stop offset of the current event
                if self.events[index].stopOffset > self.events[index].startOffset:
                    yield ("STOP", remaining(self.events[index].stopOffset), None, index)
                else: # stop is next week
                    yield ("STOP", remaining(self.events[index].stopOffset) + _ONE_WEEK_IN_SECS, None, index)
                wasStart = False
            else:
                # Go to the next event
                index += 1
                if index == len(self.events): # End of the list reached, hello new week !
                    index = 0
                    weeks +=1
                
                yield ("START", remaining(self.events[index].startOffset), self.action.getStart(self.events[index].config), index)
                wasStart = True

def parse(tree, action):
    '''
    Parse all the calendar events in the configuration and return a CalendarEventGenerator
    '''
    evg = CalendarEventGenerator(action)
    evg.parse(tree)
    return evg
