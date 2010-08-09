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


from eventgenerator import Event, CalendarEventGenerator, _ONE_WEEK_IN_SECS
import eventgenerator
import main
import math
import unittest
from main import AgentError

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
        e = Event(-1, 100, None)
        self.assertRaises(AgentError, e.check)
        # duration must be strictly positive
        e = Event(0, 0, None)
        self.assertRaises(AgentError, e.check)    
        # Too long duration
        e = Event(0, _ONE_WEEK_IN_SECS, None)
        self.assertRaises(AgentError, e.check)    
        
    def testStopOffset(self):
        ''' Check the stopOffset is correctly computed from startOffset and duration'''
        event = Event(100, 200, None)
        self.assertEqual(300, event.stopOffset)
        
    def testToString(self):
        ''' Check to string '''
        # Check it does not raise an exception
        Event(100, 200, None).__str__()
            

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
        evg._checkOverlapping()
        
        # Single event calendar 
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None)]
        evg._checkOverlapping()
        
        # No overlapping
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None), Event(200, 300, None), Event(500, 1000, None)]
        evg._checkOverlapping()

        # No overlapping but sunday / monday overlap
        evg = CalendarEventGenerator()
        evg.events = [Event(200, 100, None), Event(_ONE_WEEK_IN_SECS - 100, 200, None)]
        evg._checkOverlapping()

        # No overlapping but join
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None), Event(100, 300, None), Event(400, 1000, None)]
        evg._checkOverlapping()

        # Basic overlapping
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None), Event(50, 100, None)]
        self.assertRaises(AgentError, evg._checkOverlapping)

        # End of week overlapping
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None), Event(_ONE_WEEK_IN_SECS - 1, 100, None)]
        self.assertRaises(AgentError, evg._checkOverlapping)

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
        self.assertRaises(StopIteration, g.next)

        # One and only one event starting by start
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(0+bias, 100, None)]
        g = evg.getActions()
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS, sleepTime)
            self.assertEqual(func(), "START")
            self.assertEqual(act, "START")
        
            (act, sleepTime, func) = g.next()
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 100, sleepTime)
            self.assertEqual(act, "STOP")
            self.assertEqual(func, None)
            
        # One and only one event starting by stop
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(bias - 100, 200, None)]
        g = evg.getActions()
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 100, sleepTime)
            self.assertEqual(act, "STOP")
            self.assertEqual(func, None)
            
            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1) * eventgenerator._ONE_WEEK_IN_SECS - 100, sleepTime)
            self.assertEqual(func(), "START")
            self.assertEqual(act, "START")

        # First event is next week 
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(0, 10, None)]
        g = evg.getActions()
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals(eventgenerator._ONE_WEEK_IN_SECS - bias + (i*eventgenerator._ONE_WEEK_IN_SECS), sleepTime)
            self.assertEqual(func(), "START")
            self.assertEqual(act, "START")

            (act, sleepTime, func) = g.next()
            assert almostEquals(eventgenerator._ONE_WEEK_IN_SECS - bias + (i*eventgenerator._ONE_WEEK_IN_SECS) + 10, sleepTime)
            self.assertEqual(act, "STOP")
            self.assertEqual(func, None)
            
        # Two simple events and only one event starting by stop
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(bias + 100, 200, None), Event(bias + 1000, 400, None)]
        g = evg.getActions()
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 100, sleepTime)
            self.assertEqual(func(), "START")
            self.assertEqual(act, "START")
            
            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 300, sleepTime)
            self.assertEqual(act, "STOP")
            self.assertEqual(func, None)
            
            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 1000, sleepTime)
            self.assertEqual(func(), "START")
            self.assertEqual(act, "START")
            
            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 1400, sleepTime)
            self.assertEqual(act, "STOP")
            self.assertEqual(func, None)
            
        # Next event is next week, with week overlapping and multi event        
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(bias - 500, 100, None), Event(bias - 300, 101, None), Event(bias - 100, eventgenerator._ONE_WEEK_IN_SECS-1000, None)]
        g = evg.getActions()
 
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals(eventgenerator._ONE_WEEK_IN_SECS - 1100 + (i*eventgenerator._ONE_WEEK_IN_SECS), sleepTime)
            self.assertEqual(act, "STOP")
            self.assertEqual(func, None)

            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -500, sleepTime)
            self.assertEqual(func(), "START")
            self.assertEqual(act, "START")
                    
            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -400, sleepTime)
            self.assertEqual(act, "STOP")
            self.assertEqual(func, None)
            
            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -300, sleepTime)
            self.assertEqual(func(), "START")
            self.assertEqual(act, "START")
                    
            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -199, sleepTime)
            self.assertEqual(act, "STOP")
            self.assertEqual(func, None)   
                     
            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -100, sleepTime)
            self.assertEqual(func(), "START")
            self.assertEqual(act, "START")
                        
        # Restart
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(0+bias, 100, None), Event(100+bias, 100, None)]
        g = evg.getActions()
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS, sleepTime)
            self.assertEqual(func(), "START")
            self.assertEqual(act, "START")
                        
            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 100, sleepTime)
            assert func() == "RESTART"
        
            (act, sleepTime, func) = g.next()
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 200, sleepTime)
            self.assertEqual(act, "STOP")
            self.assertEqual(func, None)

class TestParser(unittest.TestCase):
    '''
    Check that the parser is able to get the data from the XML configuration file 
    '''
    
    def testParseFile(self):
        tree = main._parse_config_file("./eventgeneratorTest_test_parse_file.xml")
        evg = eventgenerator.parse(tree, None)
        
        # This event does not redefine the configuration
        event = evg.events[0]
        self.assertEqual("proactiveHome1", event.config.proactiveHome)
        self.assertEqual("javaHome1", event.config.javaHome)
        self.assertEqual(["param1"], event.config.jvmParameters)
        self.assertEqual(1, event.config.memoryLimit)
        self.assertEqual(1, event.config.nbRuntimes)
        self.assertEqual("script1", event.config.onRuntimeExitScript)
        self.assertEqual((6000, 6002), event.config.portRange)


        # This event redefines the whole configuration
        event = evg.events[1]
        self.assertEqual("proactiveHome2", event.config.proactiveHome)
        self.assertEqual("javaHome2", event.config.javaHome)
        self.assertEqual(["param2"], event.config.jvmParameters)
        self.assertEqual(2, event.config.memoryLimit)
        self.assertEqual(2, event.config.nbRuntimes)
        self.assertEqual("script2", event.config.onRuntimeExitScript)
        self.assertEqual((6003, 6007), event.config.portRange)
        
        # This event redefines only a part of the configuration
        event = evg.events[2]
        self.assertEqual("proactiveHome1", event.config.proactiveHome)
        self.assertEqual("javaHome3", event.config.javaHome)
        self.assertEqual(["param1"], event.config.jvmParameters)
        self.assertEqual(1, event.config.memoryLimit)
        self.assertEqual(3, event.config.nbRuntimes)
        self.assertEqual("script3", event.config.onRuntimeExitScript)
        self.assertEqual((6000, 6002), event.config.portRange)

    
    def testCoherentState(self):
        tree = main._parse_config_file("./eventgeneratorTest_test_parse_file.xml")
        evg = eventgenerator.parse(tree, None)
        evg.check()