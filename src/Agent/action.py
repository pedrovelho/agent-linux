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

from main import AgentError
import logging
import random
import main

''' 
This module is responsible for parsing the events from the agent configuration file
'''


logger = logging.getLogger("action")


def nodename_iterator():
    prefix = random.randint(1000, 9999)
    id = 0
    while True:
        yield "AGENT+%s+%s" % (prefix, id) 
        id +=1
nodename = nodename_iterator()
 
 
class _AbstractConnection:
    ''' 
    An abstract connection. It only defines the methods that must be implemented by the subclasses
    '''
    
    
    
    def __init__(self):
        self._respawn_increment = 1
        self._java_starter_class = None
        self._nodename = None
    
    def parse(self, node):
        ''' Parse the configuration file to extract the value of the connection's parameters'''
      
        lx = node.xpath("./a:nodeName", namespaces = {'a' : main.xmlns});
        assert(len(lx) == 1)
        self._nodename = lx[0].text

        lx = node.xpath("./a:javaStarterClass", namespaces = {'a' : main.xmlns});
        assert(len(lx) == 1)
        self._java_starter_class = lx[0].text

        lx = node.xpath("./a:respawnIncrement", namespaces = {'a' : main.xmlns});
        assert(len(lx) == 1)
        self._respawn_increment = lx[0].text
        
        assert(self._respawn_increment is not None)
        assert(self._respawn_increment > 0)

    def getClass(self):
        ''' Return the Java class to start '''
        raise NotImplementedError
    
    def getArguments(self):        
        ''''Return the Java arguments '''
        raise NotImplementedError
    
class DummyConnection(_AbstractConnection):
    ''' 
    A do-nothing connection. It only logs a message when a method is called
    '''
    def parse(self, doc):
        logger.debug("%s parse called" % self.__class__.__name__)

    def getClass(self):
        ''' Return the Java class to start '''
        return "org.objectweb.proactive.Main"
    
    def getArguments(self):        
        ''''Return the Java arguments '''
        return []
    
class RessourceManagerConnection(_AbstractConnection):
    ''' 
    Spawns a JVM who registers to a ProActive ressource manager
    '''
    def __init__(self):
        _AbstractConnection.__init__(self)

        self._url = None
        self._node_source_name = None
        self._credential = None
    
    
    def parse(self, node):
        _AbstractConnection.parse(self, node)
        logger.debug("%s parse called" % self.__class__.__name__)
        
        lx = node.xpath("./a:url", namespaces = {'a' : main.xmlns});
        assert(len(lx) == 1)
        self._url = lx[0].text

        lx = node.xpath("./a:nodeSourceName", namespaces = {'a' : main.xmlns});
        assert(len(lx) == 0 or len(lx) == 1)
        if len(lx) == 1:
            self._node_source_name = lx[0].text

        lx = node.xpath("./a:credential", namespaces = {'a' : main.xmlns});
        assert(len(lx) == 1)
        prop = lx[0].get("usedefault")
        if prop is "true":
            self._credential = None
        else:
            self._credential = lx[0].text

        assert(self._url is not None)
        assert(self._credential is not None)

    def getClass(self):
        return self.__javaStarterClass
    
    def getArguments(self):
        return [self.__nodeName]


class CustomConnection(_AbstractConnection):
    ''' 
    Spawns a JVM who will run user supplied code
    '''
    def __init__(self):
        _AbstractConnection.__init__(self)
      
        self._args = []
    
    def parse(self, node):
        _AbstractConnection.parse(self, node)
        logger.debug("%s parse called" % self.__class__.__name__)
        
        lx = node.xpath("./a:args/a:arg", namespaces = {'a' : main.xmlns})
        if len(lx) == 0:
            self.jvmParameters = [] 
        for node in lx:
            self._args.append(node.text)

        assert self._java_starter_class is not None, "javaStarterClass must be defined with custom connection"

    def getClass(self):
        return self.__javaStarterClass
    
    def getArguments(self):
        return [self.__nodeName]


    
class LocalBindConnection(_AbstractConnection):
    '''
    Spawns a JVM who registers into a local registery. 
    '''
    def __init__(self):
        _AbstractConnection.__init__(self)
     
    def parse(self, node):
        _AbstractConnection.parse(self, node)
        logger.debug("%s parse called" % self.__class__.__name__)
        
        if self._java_starter_class is None:
            self._java_starter_class = "org.objectweb.proactive.core.util.winagent.PAAgentServiceRMIStarter"
        
        if self._nodename is None:
            self._nodename = nodename.next()
    
    def getClass(self):
        return self.__javaStarterClass
    
    def getArguments(self):
        return [self.__nodeName]


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
