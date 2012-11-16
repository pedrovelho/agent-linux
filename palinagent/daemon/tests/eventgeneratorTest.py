#! /usr/bin/env python
# -*- coding: UTF-8 -*-
#################################################################
#
# ProActive Parallel Suite(TM): The Java(TM) library for
#    Parallel, Distributed, Multi-Core Computing for
#    Enterprise Grids & Clouds
#
# Copyright (C) 1997-2011 INRIA/University of
#                 Nice-Sophia Antipolis/ActiveEon
# Contact: proactive@ow2.org or contact@activeeon.com
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Affero General Public License
# as published by the Free Software Foundation; version 3 of
# the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
# USA
#
# If needed, contact us to obtain a release under GPL Version 2 or 3
# or a different license than the AGPL.
#
#  Initial developer(s):               The ProActive Team
#                        http://proactive.inria.fr/team_members.htm
#  Contributor(s):
#
#################################################################
# $$ACTIVEEON_INITIAL_DEV$$
#################################################################


import math
import time
import unittest
import os

from palinagent.daemon.errors import AgentError         as AgentError
from palinagent.daemon.errors import AgentInternalError as AgentInternalError

import palinagent.daemon.eventgenerator as eventgenerator
from palinagent.daemon.eventgenerator import _ONE_WEEK_IN_SECS as _ONE_WEEK_IN_SECS
from palinagent.daemon.eventgenerator import StartEvent        as StartEvent
from palinagent.daemon.eventgenerator import StopEvent         as StopEvent
from palinagent.daemon.eventgenerator import Event             as Event 
from palinagent.daemon.eventgenerator import CalendarEventGenerator as CalendarEventGenerator 

import palinagent.daemon.main as main 


def almostEquals(value1, value2, sigma=2):
    ''' Some tests use current time so strict equality cannot be used '''
    return math.fabs(value2 - value1) <= 2

class TestConfigOption(unittest.TestCase):
    pass

class Test(unittest.TestCase):
    pass

class TestConfig(unittest.TestCase):
    pass

class TestEvent(unittest.TestCase):
    ''' 
    Test the event class 
    
    It mostly checks that an exception is thrown when an invalid value is given
    or an error is encountered
    '''
    def testInit(self):
        ''' Check that the Event ctor does not accept invalid values '''
        # startOffset must be positive
        e = Event(-1, 100, None, None)
        self.assertRaises(AgentError, e.check)
        # duration must be strictly positive
        e = Event(0, 0, None, None)
        self.assertRaises(AgentError, e.check)    
        # Too long duration
        e = Event(0, _ONE_WEEK_IN_SECS, None, None)
        self.assertRaises(AgentError, e.check)    
        
    def testStopOffset(self):
        ''' Check the stopOffset is correctly computed from startOffset and duration'''
        event = Event(100, 200, None, None)
        self.assertEqual(300, event.stopOffset)
        
    def testToString(self):
        ''' Check to string '''
        # Check it does not raise an exception
        Event(100, 200, None, None).__str__()

class TestSpecificEvent(unittest.TestCase):
    
    def test_epoch_date(self):
        print(int(time.time()))
        print(time.asctime(time.localtime(time.time())))
                                          
        event = Event(0, 10, None, None)
        start_event = StartEvent(event, 0)
        print(time.asctime(time.localtime(start_event.epoch_date)))
        stop_event = StopEvent(event, 0)
        print(time.asctime(time.localtime(stop_event.epoch_date)))
        
        event = Event(_ONE_WEEK_IN_SECS - 20, 10, None, None)
        start_event = StartEvent(event, 0)
        print(time.asctime(time.localtime(start_event.epoch_date)))
        stop_event = StopEvent(event, 0)
        print(time.asctime(time.localtime(stop_event.epoch_date)))
        
        
        
class TestCalendarEventGenerator(unittest.TestCase):
    ''' 
    Test the calendar event generator
    
    The generator is configured by hand, then we check that the generator
    outputs the right events. 
    '''
    
    def testCheckOverlapping(self):
        ''' Check that an exception is raised if two events overlaps '''
        #Empty calendar
        evg = CalendarEventGenerator()
        evg.check_overlapping()
        
        # Single event calendar 
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None, None)]
        evg.check_overlapping()
        
        # No overlapping
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None, None), Event(200, 300, None, None), Event(500, 1000, None, None)]
        evg.check_overlapping()

        # No overlapping but sunday / monday overlap
        evg = CalendarEventGenerator()
        evg.events = [Event(200, 100, None, None), Event(_ONE_WEEK_IN_SECS - 100, 200, None, None)]
        evg.check_overlapping()

        # No overlapping but join
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None, None), Event(100, 300, None, None), Event(400, 1000, None, None)]
        evg.check_overlapping()

        # Basic overlapping
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None, None), Event(50, 100, None, None)]
        self.assertRaises(AgentError, evg.check_overlapping)

        # End of week overlapping
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None, None), Event(_ONE_WEEK_IN_SECS - 1, 100, None, None)]
        self.assertRaises(AgentError, evg.check_overlapping)

    def testNextEvent(self):
        ''' Test the event generator '''

        class Action:
            def getStart(self, config):
                return lambda :self._start(config)
            def _start(self, config):
                return "START"

            def getRestart(self, config):
                return lambda :self._restart(config)
            def _restart(self, config):
                return "RESTART"
        
        action = Action()
        
        # No event
        evg = CalendarEventGenerator(action)
        g  = evg.getActions()
        self.assertRaises(AgentInternalError, g.__next__)

        # One and only one event starting by start
        bias = eventgenerator._seconds_elespased_since_start_of_week()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(0+bias, 100, None, None)]
        g = evg.getActions()
        for i in range(1000):
            (event) = next(g)
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS, event.seconds_remaining())
            self.assertEqual(event.type, "START")
        
            (event) = next(g)
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 100,  event.seconds_remaining())
            self.assertEqual(event.type, "STOP")
            
        # One and only one event starting by stop
        bias = eventgenerator._seconds_elespased_since_start_of_week()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(bias - 100, 200, None, None)]
        g = evg.getActions()
        
        (event) = next(g)
        assert almostEquals(0, event.seconds_remaining())
        self.assertEqual(event.type, "START")
        
        for i in range(1000):
            (event) = next(g)
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 100, event.seconds_remaining())
            self.assertEqual(event.type, "STOP")
            
            (event) = next(g)
            assert almostEquals((i+1) * eventgenerator._ONE_WEEK_IN_SECS - 100, event.seconds_remaining())
            self.assertEqual(event.type, "START")

        # First event is next week 
        bias = eventgenerator._seconds_elespased_since_start_of_week()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(0, 10, None, None)]
        g = evg.getActions()
        for i in range(1000):
            (event) = next(g)
            assert almostEquals(eventgenerator._ONE_WEEK_IN_SECS - bias + (i*eventgenerator._ONE_WEEK_IN_SECS), event.seconds_remaining())
            self.assertEqual(event.type, "START")

            (event) = next(g)
            assert almostEquals(eventgenerator._ONE_WEEK_IN_SECS - bias + (i*eventgenerator._ONE_WEEK_IN_SECS) + 10, event.seconds_remaining())
            self.assertEqual(event.type, "STOP")
            
        # Two simple events and only one event starting by stop
        bias = eventgenerator._seconds_elespased_since_start_of_week()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(bias + 100, 200, None, None), Event(bias + 1000, 400, None, None)]
        g = evg.getActions()
        for i in range(1000):
            (event) = next(g)
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 100, event.seconds_remaining())
            self.assertEqual(event.type, "START")
            
            (event) = next(g)
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 300, event.seconds_remaining())
            self.assertEqual(event.type, "STOP")
            
            (event) = next(g)
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 1000, event.seconds_remaining())
            self.assertEqual(event.type, "START")
                        
            (event) = next(g)
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 1400, event.seconds_remaining())
            self.assertEqual(event.type, "STOP")
            
        # Next event is next week, with week overlapping and multi event        
        bias = eventgenerator._seconds_elespased_since_start_of_week()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(bias - 500, 100, None, None), Event(bias - 300, 101, None, None), Event(bias - 100, eventgenerator._ONE_WEEK_IN_SECS-1000, None, None)]
        g = evg.getActions()
 
        for i in range(1000):
            (event) = next(g)
            assert almostEquals(eventgenerator._ONE_WEEK_IN_SECS - 1100 + (i*eventgenerator._ONE_WEEK_IN_SECS), event.seconds_remaining())
            self.assertEqual(event.type, "STOP")

            (event) = next(g)
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -500, event.seconds_remaining())
            self.assertEqual(event.type, "START")
                                
            (event) = next(g)
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -400, event.seconds_remaining())
            self.assertEqual(event.type, "STOP")
            
            (event) = next(g)
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -300, event.seconds_remaining())
            self.assertEqual(event.type, "START")
                                
            (event) = next(g)
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -199, event.seconds_remaining())
            self.assertEqual(event.type, "STOP")
                     
            (event) = next(g)
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -100, event.seconds_remaining())
            self.assertEqual(event.type, "START")
                                    
        # Restart
        bias = eventgenerator._seconds_elespased_since_start_of_week()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(0+bias, 100, None, None), Event(100+bias, 100, None, None)]
        g = evg.getActions()
        for i in range(1000):
            (event) = next(g)
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS, event.seconds_remaining())
            self.assertEqual(event.type, "START")
                                    
            (event) = next(g)
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 100, event.seconds_remaining())
            self.assertEqual(event.type, "RESTART")
        
            (event) = next(g)
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 200, event.seconds_remaining())
            self.assertEqual(event.type, "STOP")

class TestParser(unittest.TestCase):
    '''
    Check that the parser is able to get the data from the XML configuration file 
    '''
    
    def testParseFile(self):
        tree = main._parse_config_file(os.path.join(os.path.dirname(__file__), "./eventgeneratorTest_test_parse_file.xml"))
        evg = eventgenerator.parse(tree, None)
        
        # This event does not redefine the configuration
        event = evg.events[0]
        self.assertEqual("proactiveHome1", event.config.proactiveHome)
        self.assertEqual("javaHome1", event.config.javaHome)
        self.assertEqual(["param1"], event.config.jvmParameters)
#        self.assertEqual(1, event.config.memoryLimit)
        self.assertEqual(1, event.config.nbRuntimes)
        self.assertEqual("script1", event.config.onRuntimeExitScript)
        self.assertEqual((6000, 6002), event.config.portRange)


        # This event redefines the whole configuration
        event = evg.events[1]
        self.assertEqual("proactiveHome2", event.config.proactiveHome)
        self.assertEqual("javaHome2", event.config.javaHome)
        self.assertEqual(["param2"], event.config.jvmParameters)
#        self.assertEqual(2, event.config.memoryLimit)
        self.assertEqual(2, event.config.nbRuntimes)
        self.assertEqual("script2", event.config.onRuntimeExitScript)
        self.assertEqual((6003, 6007), event.config.portRange)
        
        # This event redefines only a part of the configuration
        event = evg.events[2]
        self.assertEqual("proactiveHome1", event.config.proactiveHome)
        self.assertEqual("javaHome3", event.config.javaHome)
        self.assertEqual(["param1"], event.config.jvmParameters)
#        self.assertEqual(1, event.config.memoryLimit)
        self.assertEqual(3, event.config.nbRuntimes)
        self.assertEqual("script3", event.config.onRuntimeExitScript)
        self.assertEqual((6000, 6002), event.config.portRange)

    
    def testCoherentState(self):
        tree = main._parse_config_file(os.path.join(os.path.dirname(__file__), "./eventgeneratorTest_test_parse_file.xml"))
        evg = eventgenerator.parse(tree, None)
        evg.check()
        
        
if __name__ == "__main__":
    unittest.main()            