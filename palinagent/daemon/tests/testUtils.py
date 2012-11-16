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

import unittest
import subprocess

import palinagent.daemon.utils

class TestNumberOfCPU(unittest.TestCase):
    
    def test_get_number_of_cpus(self):
        n = palinagent.daemon.utils.get_number_of_cpus()
        
        try:
            p = subprocess.Popen(["lscpu", '--parse'], stdout=subprocess.PIPE)
            (stdout, stderr) = p.communicate()
            count = 0
            for line in stdout.split('\n'):
                if not line.startswith('#'):
                    count += 1
                    
            self.assertEqual(n, count - 1)
                    
        except (OSError) as e:
            print("Test disabled.lscpu not available: %s" % e)
            
if __name__ == "__main__":
    unittest.main()