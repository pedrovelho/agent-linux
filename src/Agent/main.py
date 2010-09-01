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
from lxml import etree
import logging.config
import optparse
import sys


'''
This is the ProActive Linux agent main entry point
'''


xmlns = "urn:proactive:agent:linux:3.0"
cgroup_mnt_point = "/home/cmathieu/cg"


class AgentError(Exception):
    '''
    Base class exception for the ProActive Linux agent
    '''
    def __init__(self, msg):
        self.msg = msg
        
    def __str__(self):
        return "%s" % self.msg

class AgentInternalError(AgentError):
    pass
        
def _parse_config_file(fname):
    '''
    Parse the XML configuration file and return its XML tree
    '''
    
    # Load the XML Schema and read the configuration file
    #
    # TODO: Handle user error (wrong schema, invalid XML etc.) as nicely as possible
    try:
        schemaFname = "../../../xml/agent-linux.xsd"
        schema = etree.XMLSchema(file=schemaFname)
    except etree.LxmlError as e:
        raise AgentError("Unable to load XML Schema %s: %s" % (schemaFname, e))
    
    try: 
        parser = etree.XMLParser(schema=schema, no_network=True, compact=True)
        tree = etree.parse(fname, parser)
    except IOError as e: # File not found or access right
        raise AgentError("Unable to read configuration file: %s" % fname)
    except etree.XMLSyntaxError as e: # Bad XSD declaration, Invalid or malformed XML
        raise AgentError("Unable to parse the user supplied configuration file %s: %s " % (fname, e))
    except etree.LxmlError as e: # Catch all
        raise AgentError("Unable to parse the user supplied configuration file %s: %s " % (fname, e))
    
    return tree

         
def main():
    parser = optparse.OptionParser()
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False, help="Enable verbose mode", )
    parser.add_option("-c", "--cgroup",  action="store",      dest="cgroup",  type="string", help="cgroup mount point", )
    parser.add_option("-l", "--log",     action="store",      dest="log",     type="string", help="The logging configuration file to use")
    (options, args) = parser.parse_args();

    # Configure logging
    fname = "logging.conf"
    if options.log is not None:
        fname = options.log
    logging.config.fileConfig(fname)
    if options.verbose is True:
        # TODO: Set level to DEBUG for each logger
        pass
    logger = logging.getLogger("agent")
    
    fname = "config.xml"
    if len(args) == 1:
        fname = args[0]
    elif len(args) != 0:
        parser.print_help()
        return 1 

    tree = _parse_config_file(fname)
    
    # Parse connections
    import action
    try:
        act = action.parse(tree)
    except AgentError as e:
        logger.error("Failed to parse the configuration file: %s" % e)
        return 2
    
    # Parse the calendar
    import eventgenerator
    evg =  eventgenerator.parse(tree, act)

    # Start the agent main loop
    import controller
    controller.mainloop(evg)

    return 0
    
if __name__ == '__main__':
    sys.exit(main())
