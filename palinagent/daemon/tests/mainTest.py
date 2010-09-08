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
import os

from palinagent.daemon.errors import AgentError
import palinagent.daemon.main as main

class TestXMLLoading(unittest.TestCase):
    '''
    Check that the agent correctly handle bad XML files
    '''
    
    def test_does_not_exit(self):
        ''' Checks that an exception is thrown is a wrong path is given'''
        self.assertRaises(AgentError, main._parse_config_file, "/does/not/exist")
        
    def test_bad_xsd(self):
        ''' Checks that an exception is thrown is the namespace is wrong'''
        self.assertRaises(AgentError, main._parse_config_file, os.path.join(os.path.dirname(__file__), "./mainTest_test_bad_xsd.xml"))
        
    def test_invalid_xml(self):
        ''' Checks that an exception is thrown if the file is invalid'''
        self.assertRaises(AgentError, main._parse_config_file, os.path.join(os.path.dirname(__file__), "./mainTest_test_invalid_xml.xml"))
        
    def test_valid_xml(self):
        ''' Checks that everything is fine when a valid file is given'''
        a = main._parse_config_file(os.path.join(os.path.dirname(__file__), "./mainTest_test_valid_xml.xml"))
        self.assertNotEqual(None, a)

  
if __name__ == "__main__":
    unittest.main()            