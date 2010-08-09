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
from main import AgentError
import logging
import main

''' 
This module is responsible for parsing the events from the agent configuration file
'''


logger = logging.getLogger("action")

class _AbstractConnection:
    ''' 
    An abstract connection. It only defines the methods that must be implemented by the subclasses
    '''
    def getStart(self, config):
        ''' Returns the command to executed (as a string) to start the connection'''
        raise NotImplemented
    
    def parse(self, node):
        ''' Parse the configuration file to extract the value of the connection's parameters'''
        raise NotImplementedError
                    
                    
                    
class DummyConnection(_AbstractConnection):
    ''' 
    A do-nothing connection. It only logs a message when a method is called
    '''
    def parse(self, doc):
        logger.debug("%s parse called" % self.__class__.__name__)

    def start(self, config):
        logger.debug("%s start called" % self.__class__.__name__)
        return "/bin/true"



class RessourceManagerConnection(_AbstractConnection):
    ''' 
    Spawns a JVM who registers to a ProActive ressource manager
    '''
    def parse(self, doc):
        logger.debug("%s parse called" % self.__class__.__name__)
        return "/bin/true"

    def start(self, config):
        logger.debug("%s start called" % self.__class__.__name__)
        return "/bin/true"
    


class CustomConnection(_AbstractConnection):
    ''' 
    Spawns a JVM who will run user supplied code
    '''
    def parse(self, doc):
        logger.debug("%s parse called" % self.__class__.__name__)
        return "/bin/true"

    def start(self, config):
        logger.debug("%s start called" % self.__class__.__name__)
        return "/bin/true"


    
class LocalBindConnection(_AbstractConnection):
    '''
    Spawns a JVM who registers into a local registery. 
    '''
    __nodeName = None
    __javaStarterClass = None
    __initialRestartDelay = None 
     
    def parse(self, node):
        logger.debug("%s parse called" % self.__class__.__name__)
        
        lx = node.xpath("./a:nodeName", namespaces = {'a' : main.xmlns});
        assert(len(lx) == 1)
        self.__nodeName = lx[0].text

        lx = node.xpath("./a:javaStarterClass", namespaces = {'a' : main.xmlns});
        assert(len(lx) == 1)
        self.__javaStarterClass = lx[0].text

        lx = node.xpath("./a:initialRestartDelay", namespaces = {'a' : main.xmlns});
        assert(len(lx) == 1)
        self.__initialRestartDelay = lx[0].text
        
        assert(self.__nodeName is not None)
        assert(self.__javaStarterClass is not None)
        assert(self.__nodeName is not None)
        
        return self
    
    def start(self, config):
        logger.debug("%s start called" % self.__class__.__name__)
        cmd = _buildJavaCommand(config, self.__javaStarterClass, self.__nodeName);
        return cmd


    
def _buildJavaCommand(config, className, classArg=None):
    cmd = []
    
    # Java
    cmd.append(config.getOption("javaHome").value)
    
    # Classpath
    cmd.append("-cp " + config.getOption("proactiveLocation").value +  "/dist/lib/*")
    
    # JVM options provided by the user
    # TODO add jvmParameters
    # cmd += config.getOption("jvmParameters").value
    
    # ProActive option
    cmd.append("-Dproactive.communication.protocol=" + config.getOption("protocol").value)
    
    # User

    # Memory limit
    
    
    if config.getOption("enableMemoryManagement").value is not None:
        cmd.append("-Xmx " + config.getOption("javaMemory").value)
    
    # Class
    cmd.append(className)
    cmd.append(classArg)

    return cmd
    

def parse(tree):
    '''
    Parse the agent configuration file and return an _Connection object
    
    If the file cannot be parsed an AgentException is raised
    '''
    handlers = {
                'localBind' : LocalBindConnection,
                'rmConnection': RessourceManagerConnection,
                'customConnection': CustomConnection,
                }
    
    # Delegate the parsing to the right class (according to the enabled action)
    lx = tree.xpath("//a:connections/*[@enabled='true' or @enabled='1']", namespaces = {'a' : main.xmlns})

    l = len(lx)
    if (l == 0):
        raise AgentError('Invalid configuration file. One and only one connection must be enabled (It cannot be enforced by the XML Schema)')
    elif (l == 1):
        name = lx[0].tag.replace("{%s}" % main.xmlns, "", 1)
        handler = handlers.get(name)()
        handler.parse(lx[0])
        return handler
    else:
        raise AgentError('Invalid configuration file. One connection must be enabled (It cannot be enforced by the XML Schema)')
