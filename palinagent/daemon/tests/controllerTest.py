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

import unittest
import time

import palinagent.daemon.controller as controller

import palinagent.daemon.eventgenerator as eventgenerator
from palinagent.daemon.eventgenerator import EventConfig as EventConfig
from palinagent.daemon.eventgenerator import StartEvent as StartEvent
from palinagent.daemon.eventgenerator import StopEvent as StopEvent
from palinagent.daemon.action import DummyConnection as DummyConnection

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
