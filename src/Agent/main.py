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
import optparse
import sys
import os
import logging.handlers

'''
This is the ProActive Linux agent main entry point
'''

# Some global variables.
# We should store them somewhere else, but here they are

# The current XML namespace 
xmlns = "urn:proactive:agent:linux:3.0"

# The mount point of the cgroup filesystem
cgroup_mnt_point = None

logger = None


class AgentError(Exception):
    '''
    Base class exception for the ProActive Linux agent
    '''
    def __init__(self, msg):
        self.message = msg
        
    def __str__(self):
        return "%s" % self.msg


class AgentInternalError(AgentError):
    '''
    ProActive Linux agent internal error
    
    A such exception is raised when the agent is in an unexpected or bad state. If a
    such exception is encountered it means that you do a bug report ;)
    '''
    
    def __str__(self):
        return "%s (This is an internal error, please fill a bug report)" % self.message
    
        
class AgentSetupError(AgentError):

    def __str__(self):
        return "%s (please check your installation)" % self.message
    

class AgentConfigFileError(AgentError):
    
    def __str__(self):
        return  "Invalid configuration file: %s" % self.message
    
    
        
def _parse_config_file(fname):
    '''
    Parse the XML configuration file and return its XML tree
    '''
    
    # Load the XML Schema and read the configuration file
    #
    # TODO: Handle user error (wrong schema, invalid XML etc.) as nicely as possible
    try:
        schemaFname = os.path.join(os.path.dirname(__file__), "../../xml/agent-linux.xsd")
        schema = etree.XMLSchema(file=schemaFname)
    except etree.LxmlError as e:
        raise AgentSetupError("Unable to load XML Schema %s: %s" % (schemaFname, e))
    
    try: 
        parser = etree.XMLParser(schema=schema, no_network=True, compact=True)
        tree = etree.parse(fname, parser)
    except IOError as e: # File not found or access right
        raise AgentConfigFileError("Unable to read configuration file: %s" % fname)
    except etree.XMLSyntaxError as e: # Bad XSD declaration, Invalid or malformed XML
        raise AgentConfigFileError("Unable to parse the user supplied configuration file %s: %s " % (fname, e))
    except etree.LxmlError as e: # Catch all
        raise AgentConfigFileError("Unable to parse the user supplied configuration file %s: %s " % (fname, e))
    
    return tree

def configure_logging(level, print_on_stdout, log_fname=None):
    # TODO: LINAGENT-6 Should load the configuration from a file    
    if log_fname is None:
        log_fname = os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../ProActiveAgent-log.txt")
    
    my_logger = logging.getLogger('agent')
    my_logger.setLevel(level)
    
    formatter = logging.Formatter("%(asctime)s - %(name)-10s - %(levelname)-8s - %(message)s", "%Y-%m-%d %H:%M:%S")

    
    handler = logging.handlers.RotatingFileHandler(log_fname, maxBytes=10*1024*1024, backupCount=10)
    handler.setFormatter(formatter)
    handler.doRollover() # It's cleaner to use a new file when the agent is restarted
    my_logger.addHandler(handler)

    if print_on_stdout:
        handler = logging.StreamHandler()
        handler.setFormatter(formatter)
        my_logger.addHandler(handler)
    #fname = os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../loggers.ini")
    #if options.log is not None:
    #    fname = options.log
    #logging.config.fileConfig(fname)
    
    return my_logger
         
def main():
    '''
    Linux agent entry point
    
    Parse the option, read the configurations file, and do everything
    '''
    parser = optparse.OptionParser()
    parser.add_option("-d", "--debug",   action="store_true", dest="debug",   default=False, help="Enable debug level messages", )
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False, help="Enable verbose mode (print on stdout)", )
#    parser.add_option("-l", "--logConf",     action="store",      dest="log",     type="string", help="The logging configuration file to use")
    parser.add_option("-L", "--logFile", action="store",      dest="logFile", type="string", help="Path of the log file")
    parser.add_option("-c", "--cgroup",  action="store",      dest="cgroup",  type="string", help="cgroup mount point", )
    (options, args) = parser.parse_args();

    main.cgroup_mnt_point = options.cgroup

    level = logging.INFO
    if options.debug is True:
        level = logging.DEBUG

    try :
        main.logger = configure_logging(level, options.verbose, options.logFile)
        main.logger.info ("Agent started uid=%s cgroup=%s" %(os.getuid(), main.cgroup_mnt_point))
        main.logger.debug("Agent is in debug mode !")
    except Exception as e:
        print >> sys.stderr, "Failed to configure the logging module: %s" % e
        return 3
    
    fname = os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../config.xml")
    if len(args) == 1:
        fname = args[0]
    elif len(args) != 0:
        parser.print_help()
        return 1 
    
    def print_log_exit(error):
        main.logger.critical(error)
        if not options.verbose:
            print >> sys.stderr, error
        sys.exit(5)
    
    try:
        tree = _parse_config_file(fname)
    except (AgentError) as e:
        print_log_exit(e)
        
    # Parse connections
    try:
        import action
        act = action.parse(tree)
    except (AgentError) as e:
        print_log_exit(e)

    # Parse the calendar
    try:
        import eventgenerator
        evg =  eventgenerator.parse(tree, act)
    except (AgentError) as e:
        print_log_exit(e)

    # Start the agent main loop
    import controller
    controller.mainloop(evg)

    return 0
    
if __name__ == '__main__':
    sys.exit(main())
