'''
Created on Jul 8, 2010

@author: cmathieu
'''
import unittest
from eventgenerator import Event, CalendarEventGenerator, _ONE_WEEK_IN_SECS
import eventgenerator
import math

def almostEquals(value1, value2, sigma=2):
    return math.fabs(value2 - value1) <= 2

class TestConfigOption(unittest.TestCase):
    pass

class Test(unittest.TestCase):
    pass

class TestConfig(unittest.TestCase):
    pass

class TestEvent(unittest.TestCase):
    
    def testInit(self):
        # startOffset must be positive
        self.assertRaises(AssertionError, Event, -1, 100, None)
        # duration must be strictly positive
        self.assertRaises(AssertionError, Event, 0, 0, None)    
        # Too long duration
        self.assertRaises(AssertionError, Event, 0, _ONE_WEEK_IN_SECS, None)    
        
    def testStopOffset(self):
        event = Event(100, 200, None)
        self.assertEqual(300, event.stopOffset)
        
    def testToString(self):
        # Check it does not raise an exception
        Event(100, 200, None).__str__()
            

class TestCalendarEventGenerator(unittest.TestCase):
    
    def testCheckOverlapping(self):
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
        self.assertRaises(AssertionError, evg._checkOverlapping)

        # End of week overlapping
        evg = CalendarEventGenerator()
        evg.events = [Event(0, 100, None), Event(_ONE_WEEK_IN_SECS - 1, 100, None)]
        self.assertRaises(AssertionError, evg._checkOverlapping)

    def testNextEvent(self):
        class Action:
            def getStart(self, config):
                return lambda :self._start(config)
            def _start(self, config):
                return "START"

            def getStop(self, config):
                return lambda :self._stop()
            def _stop(self):
                return "STOP"

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
            assert func() == "START"
            assert act == "START"
        
            (act, sleepTime, func) = g.next()
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 100, sleepTime)
            assert func() == "STOP"
            
        # One and only one event starting by stop
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(bias - 100, 200, None)]
        g = evg.getActions()
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 100, sleepTime)
            assert func() == "STOP"

            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1) * eventgenerator._ONE_WEEK_IN_SECS - 100, sleepTime)
            assert func() == "START"

        # First event is next week 
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(0, 10, None)]
        g = evg.getActions()
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals(eventgenerator._ONE_WEEK_IN_SECS - bias + (i*eventgenerator._ONE_WEEK_IN_SECS), sleepTime)
            assert func() == "START"

            (act, sleepTime, func) = g.next()
            assert almostEquals(eventgenerator._ONE_WEEK_IN_SECS - bias + (i*eventgenerator._ONE_WEEK_IN_SECS) + 10, sleepTime)
            assert func() == "STOP"
            
        # Two simple events and only one event starting by stop
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(bias + 100, 200, None), Event(bias + 1000, 400, None)]
        g = evg.getActions()
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 100, sleepTime)
            assert func() == "START"

            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 300, sleepTime)
            assert func() == "STOP"
            
            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 1000, sleepTime)
            assert func() == "START"

            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 1400, sleepTime)
            assert func() == "STOP"

        # Next event is next week, with week overlapping and multi event        
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(bias - 500, 100, None), Event(bias - 300, 101, None), Event(bias - 100, eventgenerator._ONE_WEEK_IN_SECS-1000, None)]
        g = evg.getActions()
 
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals(eventgenerator._ONE_WEEK_IN_SECS - 1100 + (i*eventgenerator._ONE_WEEK_IN_SECS), sleepTime)
            assert func() == "STOP"

            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -500, sleepTime)
            assert func() == "START"
        
            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -400, sleepTime)
            assert func() == "STOP"

            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -300, sleepTime)
            assert func() == "START"
        
            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -199, sleepTime)
            assert func() == "STOP"
            
            (act, sleepTime, func) = g.next()
            assert almostEquals((i+1)*eventgenerator._ONE_WEEK_IN_SECS -100, sleepTime)
            assert func() == "START"
            
        # Restart
        bias = eventgenerator.secondsSinceStartOfWeeK()
        evg = CalendarEventGenerator(action)
        evg.events = [Event(0+bias, 100, None), Event(100+bias, 100, None)]
        g = evg.getActions()
        for i in range(1000):
            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS, sleepTime)
            assert func() == "START"
            
            (act, sleepTime, func) = g.next()
            assert almostEquals(i * eventgenerator._ONE_WEEK_IN_SECS + 100, sleepTime)
            assert func() == "RESTART"
        
            (act, sleepTime, func) = g.next()
            assert almostEquals((i * eventgenerator._ONE_WEEK_IN_SECS) + 200, sleepTime)
            assert func() == "STOP"
            

class TestEventGenerator(unittest.TestCase):

    def setUp(self):
        pass
    
    def testOverlap(self):
        pass

    def testName(self):
        pass


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()