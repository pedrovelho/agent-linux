'''
Created on Jul 23, 2010

@author: cmathieu
'''

import unittest
import controller
from eventgenerator import StartEvent, StopEvent, EventConfig
import time
import eventgenerator
from action import DummyConnection

#class TestStarter(unittest.TestCase):
#    def testEchoStarter(self):
#        str = "Linux agent: it is de la boullette"
#        s = controller.Starter(["echo", "-n",  str], 10)
#        s.start()
#        
#        while s.p.poll() is None:
#            pass 
#
#        self.assertEqual(0, s.p.poll())
#        (stdout, stderr) = s.p.communicate()
#        self.assertEqual(str, stdout)
#        self.assertEqual("" , stderr)
#
#    def testOSErrorStarter(self):
#        s = controller.Starter(["dontexist", "toto"])
#        self.assertRaises(OSError, s.start)
        
class TestController(unittest.TestCase):
    
    def testBlackBox(self):
        class EventGeneratorMock():
            def getActions(self):
                config = EventConfig()
                config.javaHome      = "/home/cmathieu/java/x86/sun/jdk1.6.0_10/"
                config.proactiveHome = "/home/cmathieu/Workspaces/PPS/programming-git"
                config.memoryLimit   = 0
                config.portRange     = (2000, 3000)
                action = DummyConnection()
                
                now = int(time.time())
                yield (StartEvent(eventgenerator.Event(-1,  5, config, action), now + 5))
                yield (StopEvent( eventgenerator.Event(-1, -1, config, action), now + 15))
                yield (StartEvent(eventgenerator.Event(-1,  2, config, action), now + 20))
                yield (StopEvent( eventgenerator.Event(-1, -1, config, action), now + 25))

        evg = EventGeneratorMock()
        controller.mainloop(evg, 1, 0)                
