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


'''
This is the ProActive Linux agent main entry point
'''

import optparse
import logging.config
import sys

    
def main():
    
    parser = optparse.OptionParser()
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False, help="Enable verbose mode", )
    parser.add_option("-c", "--cgroup",  action="store",      dest="cgroup",  type="string", help="cgroup mount point", )

    (options, args) = parser.parse_args();

    logging.config.fileConfig("logging.conf")
    if options.verbose is True:
        # TODO: Set level to DEBUG for each logger
        pass
    
    filename = "config.xml"
    if len(args) == 1:
        filename = args[0]
    elif len(args) != 0:
        parser.print_help()
        return 1 
    
    import action
    import eventgenerator
    import controller
    
    act = action.parse(filename)
    evg =  eventgenerator.parse(filename, act)
    controller.mainloop(evg)

    return 0
    
if __name__ == '__main__':
    sys.exit(main())
