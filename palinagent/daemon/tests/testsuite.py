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
import sys

from palinagent.daemon.tests import xmlrunner

import palinagent.daemon.tests.testXMLConfig
import palinagent.daemon.tests.testEventGenerator
import palinagent.daemon.tests.testUtils
import palinagent.daemon.tests.testErrors


loader = unittest.TestLoader()
suite = unittest.TestSuite([
    loader.loadTestsFromModule(palinagent.daemon.tests.testXMLConfig),
    loader.loadTestsFromModule(palinagent.daemon.tests.testEventGenerator),
    loader.loadTestsFromModule(palinagent.daemon.tests.testUtils),
    loader.loadTestsFromModule(palinagent.daemon.tests.testErrors),
])

def get_testsuite():
    return suite


if __name__ == "__main__":
    import optparse
    
    parser = optparse.OptionParser()
    parser.add_option("-o", "--output",  action="store", dest="output", default="text",   help="Format of the output", )
    parser.add_option("-c", "--coverage",action="store", dest="coverage", default="None", help="Enable code coverage", )
   
    (options, args) = parser.parse_args();
    
    runner = None
    if options.output == "text":
        runner = unittest.TextTestRunner(verbosity=2)
    elif options.output == "xml":
        runner = xmlrunner.XMLTestRunner()
    else:
        print >> sys.stderr, "Unsupported unittest output format"
        sys.exit(1)
    
    cov = None
    if options.coverage is not None:
        try :
            import coverage
            try:
                cov = coverage.coverage()
            except Exception:
                # Old releases does not allow to invoke the ctor but use a singleton
                cov = coverage.the_coverage
            cov.erase()
            cov.start()
        except ImportError:
            print >> sys.stderr, "Coverage module not available"
    runner.run(suite)

    if cov is not None:
        cov.stop()
        if options.coverage == "html":
            cov.html_report(directory='covhtml')
        elif options.coverage == "xml":
            cov.xml_report(outfile='coverage.xml')
        else:
            sys.exit(1)
            print >> sys.stderr, "Unsupported coverage output format"
