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
import optparse
import sys
import os
import logging.handlers
from errors import *
import utils

try:
    from lxml import etree
except (ImportError), e:
    print >> sys.stderr, AgentSetupError("lxml not available: %s" % e)
    sys.exit(7)

'''
This is the ProActive Linux agent main entry point
'''

# Some global variables.
# We should store them somewhere else, but here they are

# The current XML namespace 
xmlns = "urn:proactive:agent:1.0:linux"

version = "1.0.0-dev"

def _parse_config_file(fname):
    '''
    Parse the XML configuration file and return its XML tree
    '''
    
    # Load the XML Schema and read the configuration file
    #
    # TODO: Handle user error (wrong schema, invalid XML etc.) as nicely as possible
    try:
        schemaFname = os.path.join(os.path.dirname(__file__), "xsd/1.0/agent-linux.xsd")
        tree = etree.parse(schemaFname)
        schema = etree.XMLSchema(tree)
    except (etree.LxmlError), e:
        raise AgentSetupError("Unable to load XML Schema %s: %s" % (schemaFname, e))
    
    try: 
        parser = etree.XMLParser(schema=schema, no_network=True, compact=True)
        tree = etree.parse(fname, parser)
    except (IOError), e: # File not found or access right
        raise AgentConfigFileError("Unable to read configuration file: %s" % fname)
    except (etree.XMLSyntaxError), e: # Bad XSD declaration, Invalid or malformed XML
        raise AgentConfigFileError("Unable to parse the user supplied configuration file %s: %s " % (fname, e))
    except (etree.LxmlError), e: # Catch all
        raise AgentConfigFileError("Unable to parse the user supplied configuration file %s: %s " % (fname, e))
    
    return tree

def configure_logging(level, print_on_stdout, log_fname=None):
    # TODO: LINAGENT-6 Should load the configuration from a file    
    if log_fname is None:
        log_fname = os.path.join(os.path.abspath(os.getcwd()), "ProActiveAgent-log.txt")
    
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
      
def _log_debug_info(logger):
    logger.debug("uname sysname:%s nodename:%s release:%s version:%s machine:%s" % os.uname())
    logger.debug("uid:%s gid:%s euid:%s egid:%s" % (os.getuid(), os.getgid(), os.geteuid(), os.getegid()))
    logger.debug("environment: %s" % (os.environ))
    logger.debug("cwd: %s" % (os.getcwd()))
    logger.debug("agent path: %s" % (os.path.dirname(os.path.abspath(__file__))))
    logger.debug("#core: %s" % utils.get_number_of_cpus())

def main_func():
    '''
    Linux agent entry point
    
    Parse the option, read the configurations file, and do everything
    '''
    usage = "usage: %prog [options] config_file"
    parser = optparse.OptionParser(usage=usage)
    parser.add_option("-d", "--debug",   action="store_true", dest="debug",   default=False, help="Enable debug level messages", )
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False, help="Enable verbose mode (print on stdout)", )
    #parser.add_option("-l", "--logConf",     action="store",      dest="log",     type="string", help="The logging configuration file to use")
    parser.add_option("-L", "--logFile", action="store",      dest="logFile", type="string", help="Path of the log file")
    parser.add_option("-V", "--version", action="store_true", dest="version", default=False, help="Print version information")

    (options, args) = parser.parse_args();

    if options.version is True:
        print version
        return 0

    if len(args) == 1:
        fname = args[0]
    elif len(args) == 0:
        fname = "/etc/proactive/agent.xml"
    else:
        parser.print_help()
        return 1 

    try :
        level = logging.INFO
        if options.debug is True:
            level = logging.DEBUG
        
        logger = configure_logging(level, options.verbose, options.logFile)
        logger.info ("Agent started using config file: %s" % fname)
        logger.debug("Agent is in debug mode !")
        if options.debug is True:
            _log_debug_info(logger)
    except (Exception), e:
        print >> sys.stderr, "Failed to configure the logging module: %s" % e
        return 3
    
    
    def print_log_exit(error):
        logger.critical(error)
        if not options.verbose:
            print >> sys.stderr, error
        sys.exit(5)
    
    try:
        tree = _parse_config_file(fname)
    except (AgentError), e:
        print_log_exit(e)
        
    # Parse connections
    try:
        import action
        act = action.parse(tree)
    except (AgentError), e:
        print_log_exit(e)

    # Parse the calendar
    try:
        import eventgenerator
        evg =  eventgenerator.parse(tree, act)
    except AgentError, e:
        print_log_exit(e)


    # Start the agent main loop
    import controller
    ctrl = controller.Controller(evg)
    import signal
    
    def sigfunc(ctrl):
        def call_ctrl(signum, frame):
            logger.info("Received signal: %s. Exiting !" % signum)
            ctrl.stop()
        return call_ctrl
    
    func = sigfunc(ctrl)    
    
    for sig in [signal.SIGINT, signal.SIGQUIT, signal.SIGHUP,  signal.SIGTERM, signal.SIGILL, signal.SIGABRT, signal.SIGPIPE, signal.SIGALRM]:
        signal.signal(sig, func)
    ctrl.start()
    return 0
    
if __name__ == '__main__':
    sys.exit(main_func())
