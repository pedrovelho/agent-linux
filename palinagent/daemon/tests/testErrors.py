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

import palinagent.daemon.errors

class TestErrors(unittest.TestCase):
    
    def test_error(self):
        self.assertRaises(palinagent.daemon.errors.AgentError, raise_it, palinagent.daemon.errors.AgentError, "Oops")

    def test_config_file_error(self):
        self.assertRaises(palinagent.daemon.errors.AgentConfigFileError, raise_it, palinagent.daemon.errors.AgentConfigFileError, "Oops")
        self.assertRaises(palinagent.daemon.errors.AgentError, raise_it, palinagent.daemon.errors.AgentConfigFileError, "Oops")

    def test_internal_error(self):
        self.assertRaises(palinagent.daemon.errors.AgentInternalError, raise_it, palinagent.daemon.errors.AgentInternalError, "Oops")
        self.assertRaises(palinagent.daemon.errors.AgentError, raise_it, palinagent.daemon.errors.AgentConfigFileError, "Oops")

    def test_setup_error(self):
        self.assertRaises(palinagent.daemon.errors.AgentSetupError, raise_it, palinagent.daemon.errors.AgentSetupError, "Oops")
        self.assertRaises(palinagent.daemon.errors.AgentError, raise_it, palinagent.daemon.errors.AgentConfigFileError, "Oops")

def raise_it(error, msg):
    raise error(msg)

if __name__ == "__main__":
    unittest.main()