'''
Created on Jul 23, 2010

@author: cmathieu
'''

import unittest
import controller

class TestStarter(unittest.TestCase):
    def testEchoStarter(self):
        str = "Linux agent: it is de la boullette"
        s = controller.Starter(["echo", "-n",  str], 10)
        s.start()
        
        while s.p.poll() is None:
            pass 

        self.assertEqual(0, s.p.poll())
        (stdout, stderr) = s.p.communicate()
        self.assertEqual(str, stdout)
        self.assertEqual("" , stderr)

    def testOSErrorStarter(self):
        s = controller.Starter(["dontexist", "toto"])
        self.assertRaises(OSError, s.start)
        
class TestController(unittest.TestCase):
    
    def testBlackBox(self):
        class EventGeneratorMock():
            def getActions(self):
                yield ("START", 5, lambda: ("sleep", "1"))
                yield ("STOP",  5, lambda: "")
                yield ("START", 5, lambda: ("sleep", "10"))
                yield ("STOP",  5, lambda: "")

        evg = EventGeneratorMock()
        controller.mainloop(evg, 1, 0)                
