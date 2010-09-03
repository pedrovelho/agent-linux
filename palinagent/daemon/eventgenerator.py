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

import time
import main
import os
import subprocess
import io
import threading 
import utils
import logging
from IN import AF_INET
import sys
import errors
import signal
import errno

logger = logging.getLogger("agent.evg")

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
        self.cgroup_mnt_point = None
        self.nbRuntimes = 1
        self.protocol = "rmi"
        self.portRange = (1025, 65535)
        self.onRuntimeExitScript = None
        self.nice = 0
        self.ionice = None
        
    def check(self):
        '''
        Check the state of this event is consistent 
        '''
        if self.proactiveHome is None:
            raise errors.AgentConfigFileError("ProActive home is not set")
        if len(self.proactiveHome) < 1:
            # FIXME: Should be enforced by the XSD
            raise errors.AgentConfigFileError("ProActive home cannot be empty")
        
        if self.javaHome is None:
            raise errors.AgentConfigFileError("Java home is not set")
        if len(self.javaHome) < 1:
            # FIXME: Should be enforce by the XSD
            raise errors.AgentConfigFileError("Java home cannot be empty")
        
        if self.memoryLimit < 0:
            raise errors.AgentInternalError("Memory limit must be a positive integer")
        if self.memoryLimit != 0:
            if self.cgroup_mnt_point is None:
                raise errors.AgentInternalError("cgroup mount point is not defined but memory limit is set")
            if len(self.cgroup_mnt_point) == 0:
                raise errors.AgentInternalError("cgroup mount point lenght is 0 but memory limit is set")
            if not os.path.exists(self.cgroup_mnt_point):
                raise errors.AgentConfigFileError("cgroup is not mounted at %s" % self.cgroup_mnt_point)
            
        if self.nbRuntimes < 0:
            raise errors.AgentInternalError("The number of runtimes must be a positive integer")
        
        if self.protocol is None:
            raise errors.AgentInternalError("ProActive communication protocol is not set")
        if len(self.protocol) < 1:
            raise errors.AgentConfigFileError("ProActive communication protocol cannot be empty") 
                
        if self.portRange[0] < 0 or self.portRange[0] > 65536:
            raise errors.AgentInternalError("First TCP port must be an integer between 0 and 65536")
        if self.portRange[1] < 0 or self.portRange[1] > 65536:
            raise errors.AgentInternalError("Last TCP port must be an integer between 0 and 65536")
        if self.portRange[0] > self.portRange[1]:
            raise errors.AgentInternalError("Last TCP port must be greater or equals to the first TCP port")
        if self.portRange[1] - self.portRange[0] < self.nbRuntimes:
            raise errors.AgentInternalError("The port range is too small according to the number of runtimes")
            
        if self.nice < -20 or self.nice > 19:
            raise errors.AgentInternalError("Invalid nice value. Must be betweeon -20 and 19")
            
        if self.ionice is not None:
            if self.ionice["class"] == 1 or self.ionice["class"] == 2:
                if self.ionice["classdata"] is None:
                    raise errors.AgentConfigFileError("A class data is mandatory with the best effort and real time ionice classes")
            else:
                if self.ionice["classdata"] is not None:
                    raise errors.AgentConfigFileError("none and idle ionice classes does not accept any class data argument")
                    
    def parse(self, eventNode):
        '''
        Fill this config with the value extracted from XML configuration file
        '''
        confNode = eventNode.xpath("/a:agent/a:config", namespaces = {'a' : main.xmlns})
        assert len(confNode) == 1 
        self.__parse_config(True, confNode[0])

        eventNode = eventNode.xpath("./a:config", namespaces = {'a' : main.xmlns})
        if len(eventNode) == 1:
            self.__parse_config(False, eventNode[0])

    def __parse_config(self, isRoot, confNode):
        lx = confNode.xpath("./a:proactiveHome", namespaces = {'a' : main.xmlns})
        if isRoot: # This element is mandatory in the root config but optional otherwise
            assert len(lx) == 1
            self.proactiveHome = lx[0].text
        else:
            assert len(lx) == 1 or len(lx) == 0
            if len(lx) == 1:
                self.proactiveHome = lx[0].text
                
        lx = confNode.xpath("./a:javaHome", namespaces = {'a' : main.xmlns})
        if isRoot: # This element is mandatory in the root config but optional otherwise
            assert len(lx) == 1
            self.javaHome = lx[0].text
        else:
            assert len(lx) == 1 or len(lx) == 0
            if len(lx) == 1:
                self.javaHome = lx[0].text

        lx = confNode.xpath("./a:jvmParameters/a:param", namespaces = {'a' : main.xmlns})
        if len(lx) != 0 and not isRoot: 
            self.jvmParameters = []
        for node in lx:
            self.jvmParameters.append(node.text)

        lx = confNode.xpath("./a:nbRuntimes", namespaces = {'a' : main.xmlns})
        assert len(lx) == 1 or len(lx) == 0
        if len(lx) == 1: 
            self.nbRuntimes = int(lx[0].text)
            if self.nbRuntimes == "auto":
                self.nbRuntimes = utils.get_number_of_cpus()

        lx = confNode.xpath("./a:memoryLimit", namespaces = {'a' : main.xmlns})
        assert len(lx) == 1 or len(lx) == 0
        if len(lx) == 1:
            # Mimic the Windows agent behavior, until next release
            self.memoryLimit = int(lx[0].text) * self.nbRuntimes
            self.cgroup_mnt_point = lx[0].get("cgroup_mount_point")
        


        lx = confNode.xpath("./a:protocol", namespaces = {'a' : main.xmlns})
        assert len(lx) == 1 or len(lx) == 0
        if len(lx) == 1: 
            self.protocol = lx[0].text

        lx = confNode.xpath("./a:portRange", namespaces = {'a' : main.xmlns})
        assert len(lx) == 1 or len(lx) == 0
        if len(lx) == 1: 
            self.portRange = (int(lx[0].get("first")), int(lx[0].get("last")))

        lx = confNode.xpath("./a:onRuntimeExitScript", namespaces = {'a' : main.xmlns})
        assert len(lx) == 1 or len(lx) == 0
        if len(lx) == 1: 
            self.onRuntimeExitScript = lx[0].text

        lx = confNode.xpath("./a:nice", namespaces = {'a' : main.xmlns})
        assert len(lx) == 1 or len(lx) == 0
        if len(lx) == 1: 
            self.nice = int(lx[0].text)
            
        lx = confNode.xpath("./a:ionice", namespaces = {'a' : main.xmlns})
        assert len(lx) == 1 or len(lx) == 0
        if len(lx) == 1:
            classes = {'none' : 0, 'realtime' : 1, 'besteffort' : 2, 'idle' : 3}
            clazz = classes[lx[0].get("class")]
            data = lx[0].get("data")
            self.ionice = {"class" : clazz, "classdata" : data}
            

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
        time.sleep(max(0, self.seconds_remaining()))

    def seconds_remaining(self):
        '''
        Return the number of seconds remaining until the event
        '''
        return self.epoch_date - int(time.time())

class JVMStarter():
    '''
    This class is in  charge of spawning a new process for the JVM.
    '''
    def __init__(self, config, cmd, epoch_date, respawn_increment, rank, cgroup_mnt_point):
        self.config = config
        self.cmd = cmd
        self.epoch_date = epoch_date
        self.respawn_increment = respawn_increment
        self.rank = rank
        self.cgroup_mnt_point = cgroup_mnt_point

        self.canceled = threading.Event()
      
    def schedule(self):
        sleep_time = int(self.epoch_date - time.time())
        logger.info("Thread created to execute %s. Execution starts in %s seconds" % (self.cmd, sleep_time)) 
        self.canceled.wait(sleep_time)
       
        def get_wait_time_func(respawn_increment, max_wait_time=3*60):
            nb_die = 0
            while  True:
                nb_die += 1
                yield (nb_die, min(nb_die * respawn_increment, max_wait_time))

        
        wait_time_gen = get_wait_time_func(self.respawn_increment)
        self.p = None
        pgid = None
        while not self.canceled.isSet():
            # Do we need to restart the process ? 
            if self.p is None or self.p.poll() is not None:
                if self.p is not None:
                    try:
                        os.killpg(pgid, signal.SIGKILL)
                    except OSError as e:
                        if e.errno == errno.ESRCH:
                            pass # Ok, no process to be killed
                        else:
                            logger.warning("Killing processes belonging to process group %s (pid:%s) failed: %s" % (pgid, pid, e))
                    (nb_die, wait_time) = wait_time_gen.next()
                    logger.warning("Process %s exited with status code %s. Failure number %d waiting %s seconds before restarting" % (self.p.pid, self.p.poll(), nb_die, wait_time))
                    (stdout, stderr) = self.p.communicate()
                    logger.info("Standard output of %s is: %s" % (self.p.pid, stdout))
                    logger.info("Standard error of %s is: %s" % (self.p.pid, stderr))
                    self.canceled.wait(wait_time)
                    
                
                self.p = subprocess.Popen(self.cmd, bufsize=4096, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, preexec_fn=self._pinit)
                pgid = os.getpgid(self.p.pid)
                logger.info("Forked the process:%s to start command:%s" % (self.p.pid, self.cmd))
            self.canceled.wait(1)

        if self.p is not None:   
            self.p.terminate()
            os.killpg(pgid, signal.SIGKILL)
            logger.info("Terminated pid: %s" % self.p.pid)
            (stdout, stderr) = self.p.communicate()
            logger.info("Standard output of %s is:\n %s" % (self.p.pid, stdout))
            logger.info("Standard error  of %s is:\n %s" % (self.p.pid, stderr))
        logger.debug("Thread to execute %s exited" % (self.cmd)) 

    def cancel(self):
        self.canceled.set()
        
    def _pinit(self):
        # Do not use logger in this function since it is executed in a forked process
        # stdout & stderr are monitored by the agent
        
        # CPU Nice
        if self.config.nice != 0:
            try:
                os.nice(self.config.nice) # Can throw an OSError   
            except OSError as e:
                print >> sys.stderr, "ERROR: nice call failed"
                os._exit(253) # Don't use sys.exit() here
                
        # IONice
        if self.config.ionice is not None:
            io_class = self.config.ionice['class']
            ion_cmd  = ["/usr/bin/ionice", "-p %s" % os.getpid() , "-c %s" % io_class]
            if self.config.ionice['classdata'] is not None:
                ion_cmd.append("-n" % self.config.ionice['classdata'])
            try:
                retcode = subprocess.call(ion_cmd)
                assert retcode == 0
            except OSError as e:
                print >> sys.stderr, "ERROR: ionice call failed: %s" % e
                os._exit(253) # Don't use sys.exit() here
                
        # Memory limit
        if self.config.memoryLimit > 0:
            try:
                group = "agent"
                p = os.path.join(self.cgroup_mnt_point, group)
                m = os.path.join(p, "memory.limit_in_bytes")
                with io.open(m, "w") as file:
                    file.write(unicode(self.config.memoryLimit*1024*1024))
                        
                m = os.path.join(p, "memory.swappiness")
                with io.open(m, "w") as file:
                    file.write(unicode(0))
                    
                m = os.path.join(p, "memory.memsw.limit_in_bytes")
                with io.open(m, "w") as file:
                    file.write(unicode(self.config.memoryLimit*1024*1024))
                    
                m = os.path.join(p, "tasks")
                with io.open(m, "a") as file:
                    file.write(unicode("%d" % os.getpid()))
                    
            except IOError as e:
                # TODO: Better diagnostic / error handling
                print >> sys.stderr, "ERROR: Failed to configure cgroup to limit the available memory"
                os._exit(253) # Don't use sys.exit() here

        os.setpgrp()
        
class StartEvent(SpecificEvent):
   
    def __init__(self, generic_event, epoch_date):
        super(StartEvent, self).__init__(generic_event, epoch_date, "START")
        self.forks = []
   
    def schedule(self):
        (ports) = self._chose_tcp_ports()

        for i in xrange(self.config.nbRuntimes):
            cmd = self._build_java_cmd(i, ports[i], i)
            
            starter = JVMStarter(self.config, cmd, self.epoch_date, self.action._respawn_increment, i, self.config.cgroup_mnt_point)
            thread = threading.Thread(None, starter.schedule, "THREAD%s" % i, (), {})
            thread.start()
            self.forks.append((starter, thread))
    
    def cancel(self):
        for (starter, thread) in self.forks:
            starter.cancel() 

        for (starter, thread) in self.forks:
            thread.join()

    def _chose_tcp_ports(self):
        import socket

        ports = []
        offset = 0
        while len(ports) != self.config.nbRuntimes:
            port = self.config.portRange[0] + offset
            assert port <= self.config.portRange[1], "Port range is too small, only %d port available %s" % (len(ports), port)
            try:
                s = socket.socket(AF_INET, socket.SOCK_STREAM)
                s.bind(("", port))
                ports.append(port)
            except socket.error:
                pass
            offset +=1
        
        return ports
              
    def _build_java_cmd(self, id, tcp_port, rank):
        cmd = []
        
        # Java
        cmd.append(self.config.javaHome + "/bin/java")
        # Classpath
        cmd.append("-classpath")
        cmd.append(self.config.proactiveHome +  "/dist/lib/*")
            
        cmd.append("-Dproactive.communication.protocol=%s" % self.config.protocol)

    
        if self.config.protocol == "rmi":
            cmd.append("-Dproactive.rmi.port=%d" % tcp_port)
        elif self.config.protocol == "http":
            cmd.append("-Dproactive.http.port=%d" % tcp_port)
        elif self.config.protocol == "pamr":
            pass # PAMR does not user server socket
        elif self.config.protocol == "pnp":
            cmd.append("-Dproactive.pnp.port=%d" % tcp_port)
        else:
            print "TCP port is not supported for protocol: %s" % self.config.protocol

        cmd.append("-Djava.security.manager")
        cmd.append("-Djava.security.policy="     + os.path.join(os.path.dirname(__file__), "data/agent.java.policy"))
#       cmd.append("-Dlog4j.configuration=file:" + os.path.join(os.path.dirname(__file__), "../../agent.log4j"))
        cmd.append("-Dproactive.agent.rank=%d" % rank)
        for param in self.config.jvmParameters:
            cmd.append(param.replace('${rank}', "%d" % rank))

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
                raise errors.AgentInternalError("Invalid event state")
            else:
                raise errors.AgentError("Invalid event state: " + message)
  
        if self.startOffset < 0:
            raise errors.AgentInternalError("Start offset must be positive")
        if self.startOffset > _ONE_WEEK_IN_SECS:
            raise errors.AgentInternalError("Start offset must be lesser than %s" % _ONE_WEEK_IN_SECS)
        if self.duration < 1:
            raise errors.AgentConfigFileError("An event must last a least one second (not enforced by XSD)")
        if self.duration >= _ONE_WEEK_IN_SECS:
            raise errors.AgentInternalError("An event cannot last more than 7 days (not enforced by XSD)")
        
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
        assert len(lx) == 1

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
                    raise errors.AgentConfigFileError("Calendar events %s and %s overlap (not enforced by XSD)" % (self.events[i], self.events[i+1]))

            if (self.events[-1].stopOffset <= self.events[-1].startOffset) and (self.events[-1].stopOffset > self.events[0].startOffset):
                raise errors.AgentConfigFileError("Calendar events %s and %s overlap (not enforced by XSD)" % (self.events[-1], self.events[0]))
      
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
            raise errors.AgentInternalError("No calendars event")

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
    evg.check()
    return evg
