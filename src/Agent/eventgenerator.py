'''
Created on Jul 7, 2010

@author: cmathieu
'''

from lxml import etree
import logging
import time

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
        now =  time.localtime()
        return now[5] + (now[4]*60) + (now[3]*60*60) + (now[6]*60*60*24)


class ConfigOption(object):
    def __init__(self, name, xpath, optional=False, value=None):
        self.name = name
        self.xpath = xpath
        self.optional = optional
        self.value = value
    
    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return self.name == other.name
        else:
            return False
        
    def __str__(self):
        return "name: %s, xpath: %s, optional: %s, value: %s" % (self.name, self.xpath, self.optional, self.value)
    
    def setValue(self, value):
        self.value = value
        
    def loadFromXPath(self, eventNode):
        logging.debug("loading %s" % self)
        lx = eventNode.xpath(self.xpath, namespaces={'a' : "urn:proactive:agent:1.0"})
        assert(len(lx) <= 1 and (len(lx) == 1 or self.optional))
        if len(lx) == 1:
            self.value = lx[0].text
            
        
class Config(object):
    def __init__(self):
        self.options = []    
        self.__addOption("proactiveLocation", "/a:agent/a:internalConfig/a:proactiveLocation")
        self.__addOption("javaHome", "/a:agent/a:internalConfig/a:java_home")
        self.__addOption("jvmParameters", "/a:agent/a:internalConfig/a:jvm_parameters")
        self.__addOption("enableMemoryManagement", "/a:agent/a:internalConfig/a:enable_memory_management")
        self.__addOption("javaMemory", "/a:agent/a:internalConfig/a:java_memory")
        self.__addOption("nativeMemory", "/a:agent/a:internalConfig/a:java_memory")
        self.__addOption("nbProcesses", "/a:agent/a:internalConfig/a:nb_processes")
        self.__addOption("useAllCpus", "/a:agent/a:internalConfig/a:use_all_cpus")
        self.__addOption("protocol", "/a:agent/a:internalConfig/a:protocol")
        self.__addOption("portInitialValue", "/a:agent/a:internalConfig/a:port_initial_value")
        self.__addOption("processPriority", "./a:processPriority")
        self.__addOption("maxCpuUsage", "./a:maxCpuUsage")
        
    def __addOption(self, name, xmlName=None, optional=False):
        self.options.append(ConfigOption(name, xmlName, optional))
        
    def __str__(self):
        return "%s" % self.options
    
    def check(self):
        for option in self.options:
            if not option.optional and option.value is None:
                raise Exception("Option must be set")
    
    def loadFromEventNode(self, eventNode):
        for option in self.options:
            option.loadFromXPath(eventNode)

    def getOption(self, name):
        for option in self.options:
            if option.name == name: return option
        return None

class Event(object):
    def __init__(self, startOffset, duration, config):
        assert startOffset >= 0, "Calendar event start offset must be positive"
        assert startOffset < _ONE_WEEK_IN_SECS, "Calendar event start offset must lesser than %d" % _ONE_WEEK_IN_SECS
        assert duration > 0, "Calendar event duration must be positive"
        assert duration < _ONE_WEEK_IN_SECS, "A calendar event cannot last more than one week"
       
        self.startOffset = startOffset
        self.duration = duration
        self.config = config
    
    @property
    def stopOffset(self):
        stopOffset =  self.startOffset + self.duration
        if stopOffset > _ONE_WEEK_IN_SECS:
            stopOffset -= _ONE_WEEK_IN_SECS
        return stopOffset
        
    def __str__(self):
         return "startOffset: %s, duration: %s, config: %s" % (self.startOffset, self.duration, self.config)   

   
     
class CalendarEventGenerator(object):
    
    def __init__(self, action=None):
        self.events = []
        self.action = action

    def __str__(self):
        return "%s" % self.events[0]
    
    def __getTagValue(self, node, tagName, mandatory=True):
        lx = node.xpath("./a:%s" % tagName, namespaces={'a' : "urn:proactive:agent:1.0"})
        assert(len(lx) <= 1 and (len(lx) == 1 or not mandatory))
        if (len(lx) > 0):
            return lx[0].text
        return None  

    def __parseEvents(self, eventsNode):
        lx = eventsNode.xpath("./a:event", namespaces={'a' : "urn:proactive:agent:1.0"})
        for eventNode in lx:  
            assert(eventNode.get("type") == "CalendarEvent")
        
            config = Config()
            config.loadFromEventNode(eventNode)
            config.check()
            
            startDay = self.__getTagValue(eventNode, "startDay")
            startHour = self.__getTagValue(eventNode, "startHour")
            startMinute = self.__getTagValue(eventNode, "startMinute")
            startSecond = self.__getTagValue(eventNode, "startSecond")
            durationDays = self.__getTagValue(eventNode, "durationDays")
            durationHours = self.__getTagValue(eventNode, "durationHours")
            durationMinutes = self.__getTagValue(eventNode, "durationMinutes")
            durationSeconds = self.__getTagValue(eventNode, "durationSeconds")
        
            startOffset = int(startSecond) + (int(startMinute) * 60) + (int(startHour) * 3600) + (days[startDay] * 86400)
            duration = int(durationSeconds) + (int(durationMinutes) * 60) + (int(durationHours) * 3600) + (int(durationDays) * 86400)
            
            event = Event(startOffset, duration, config)
            self.events.append(event)
        
        # Sort events by start date    
        self.events.sort(key=lambda event: event.startOffset)
            
            
    def parse(self, filename):    
        root = etree.parse(filename)
        lx = root.xpath("/a:agent/a:events", namespaces={'a' : "urn:proactive:agent:1.0"})
        assert(len(lx) == 1)
        self.__parseEvents(lx[0])
        self._checkOverlapping()
        
        return self
    
    def _checkOverlapping(self):
        if len(self.events) > 1:
            for i in range(len(self.events)-1):
                assert self.events[i].stopOffset <= self.events[i+1].startOffset, "Calendar event %s and %s overlap" % (self.events[i], self.events[i+1])

            assert (self.events[-1].stopOffset > self.events[-1].startOffset) or (self.events[-1].stopOffset <= self.events[0].startOffset), "Calendar event %s and %s overlap" % (self.events[-1], self.events[0])
      
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
            yield ("STOP", remaining(self.events[index].stopOffset), self.action.getStop(self.events[index].config), index)
        
        if mustResetWeeks: 
            weeks = 0
            
        # Subsequent calls
        # Since no overlapping is allowed we just have to loop over the events
        while True:  
            if wasStart:
                # Return the stop offset of the current event
                if self.events[index].stopOffset > self.events[index].startOffset:
                    yield ("STOP", remaining(self.events[index].stopOffset), self.action.getStop(self.events[index].config), index)
                else: # stop is next week
                    yield ("STOP", remaining(self.events[index].stopOffset) + _ONE_WEEK_IN_SECS, self.action.getStop(self.events[index].config), index)
                wasStart = False
            else:
                # Go to the next event
                index += 1
                if index == len(self.events): # End of the list reached, hello new week !
                    index = 0
                    weeks +=1
                
                yield ("START", remaining(self.events[index].startOffset), self.action.getStart(self.events[index].config), index)
                wasStart = True

def parse(filename, action):
    return CalendarEventGenerator(action).parse(filename)
