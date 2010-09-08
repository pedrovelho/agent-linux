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
import xmlrunner
import sys

import palinagent.daemon.tests.actionTest
import palinagent.daemon.tests.controllerTest
import palinagent.daemon.tests.eventgeneratorTest
#import palinagent.daemon.tests.mainTest

suite = unittest.TestSuite([
                            unittest.TestLoader().loadTestsFromModule(palinagent.daemon.tests.actionTest),
                            unittest.TestLoader().loadTestsFromModule(palinagent.daemon.tests.controllerTest),
                            unittest.TestLoader().loadTestsFromModule(palinagent.daemon.tests.eventgeneratorTest),
#                            unittest.TestLoader().loadTestsFromModule(palinagent.daemon.tests.mainTest),
])

if __name__ == '__main__':
    runner = xmlrunner.XMLTestRunner(sys.stderr)
    runner.run(suite)
