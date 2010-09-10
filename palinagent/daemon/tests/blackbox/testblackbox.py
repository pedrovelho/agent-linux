#! /usr/bin/env python
# -*- coding: UTF-8 -*-
#################################################################
#
# ProActive Parallel Suite(TM): The Java(TM) library for
#    Parallel, Distributed, Multi-Core Computing for
#     Enterprise Grids & Clouds
#
# Copyright (C) 1997-2010 ActiveEon 
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
#  Initial developer(s):               The ActiveEon Team
#  Contributor(s):
#
#################################################################
# $$ACTIVEEON_INITIAL_DEV$$
#################################################################

import unittest
import tempfile
import os
import time
import shutil
import subprocess
import sys
import signal

class TestBlackBox(unittest.TestCase):

 
    def setUp(self):
        self.tmp_dir = tempfile.mkdtemp()
        print self.tmp_dir
        xml_fname = os.path.join(os.path.dirname(__file__), "testblackbox.xml")

        input = open(xml_fname, 'r')
        txt = input.read()
        
        os.makedirs(os.path.join(self.tmp_dir, "dist", "lib"))
        shutil.copy(os.path.join(os.path.dirname(__file__), "BlackBoxStarter.jar"), os.path.join(self.tmp_dir, "dist", "lib", "BlackBoxStarter.jar"))
        txt = txt.replace("%%PROACTIVE_HOME%%", "%s" % self.tmp_dir)
        
        txt = txt.replace("%%START_NOW_MINUS_20_SECS%%", "<start day=\"%s\" hour=\"%s\" minute=\"%s\" second=\"%s\"  />" % self.__get_split_date(-20))
        txt = txt.replace("%%START_NOW_MINUS_10_SECS%%", "<start day=\"%s\" hour=\"%s\" minute=\"%s\" second=\"%s\"  />" % self.__get_split_date(-10))
        txt = txt.replace("%%START_NOW_PLUS_10_SECS%%", "<start day=\"%s\" hour=\"%s\" minute=\"%s\" second=\"%s\"  />"  % self.__get_split_date( 10))
        txt = txt.replace("%%START_NOW_PLUS_20_SECS%%", "<start day=\"%s\" hour=\"%s\" minute=\"%s\" second=\"%s\"  />"  % self.__get_split_date( 20))
        input.close()
        
        output =  open(os.path.join(self.tmp_dir, "agent.xml"), 'w')
        output.write(txt)
        output.close()
        
    def test_blackbox(self):
        
        args = []
        args.append(sys.executable)
        args.append(os.path.join(os.path.dirname(__file__), "../../main.py"))
        args.append("-d")
        args.append("-v")
        args.append("-L")
        args.append(os.path.join(self.tmp_dir, "agent.log"))
        args.append(os.path.join(self.tmp_dir, "agent.xml"))
        print args
        p = subprocess.Popen(args)
        
        time.sleep(40)
        
        os.kill(p.pid, signal.SIGKILL) # 2.6: p.terminate()
        (out, err) = p.communicate()
        print "exitStatus: %s" % p.poll()
        print "out: %s" % out
        print "err: %s" % err
        
    def tearDown(self): 
        shutil.rmtree(self.tmp_dir)
    
    def __get_split_date(self, second_bias):
        days = {0 : "Monday", 1 : "Tuesday", 2 : "Wednesday", 3 : "Thursday", 4 : "Friday", 5 : "Saturday", 6 : "Sunday" }
        now = time.time()
        lnow = time.localtime(now + second_bias)
        return (days[lnow[6]], lnow[3], lnow[4], lnow[5])
    
if __name__ == "__main__":
    unittest.main()    