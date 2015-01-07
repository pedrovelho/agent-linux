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

import random
import logging
import main
import errors

''' 
This module is responsible for parsing the events from the agent configuration file
'''

logger = logging.getLogger("agent.act")

def _nodename_iterator():
    prefix = random.randint(1000, 9999)
    cnt = 0
    while True:
        yield "AGENT_%s_%s" % (prefix, cnt) 
        cnt +=1
        
_nodename = _nodename_iterator()
 
 
class _AbstractConnection(object):
    ''' 
    An abstract connection. It only defines the methods that must be implemented by the subclasses
    '''
    
    def __init__(self):
        self.respawn_increment = 1
        self.java_starter_class = None
        self.nodename = None
    
    def parse(self, node):
        ''' Parse the configuration file to extract the value of the connection's parameters'''
      
        lx = node.xpath("./a:nodename", namespaces = {'a' : main.xmlns})
        if len(lx) == 1:
            self.nodename = lx[0].text

        lx = node.xpath("./a:javaStarterClass", namespaces = {'a' : main.xmlns})
        if len(lx) == 1:
            self.java_starter_class = lx[0].text

        lx = node.xpath("./a:respawnIncrement", namespaces = {'a' : main.xmlns})
        if len(lx) == 1:
            self.respawn_increment = int(lx[0].text)

    def getClass(self):
        ''' Return the Java class to start '''
        raise errors.AgentInternalError("Not implemented")
    
    def getArguments(self):        
        ''''Return the Java arguments '''
        raise errors.AgentInternalError("Not implemented")
    
    
class DummyConnection(_AbstractConnection):
    ''' 
    A do-nothing connection. It only logs a message when a method is called
    '''
    def parse(self, node):
        logger.debug("%s parse called on node:%s" % (self.__class__.__name__, node))

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

        self.url = None
        self.node_source_name = None
        self.credential = None
    
    def parse(self, node):
        _AbstractConnection.parse(self, node)
        logger.debug("%s parse called" % self.__class__.__name__)
              
        lx = node.xpath("./a:url", namespaces = {'a' : main.xmlns})
        if len(lx) == 1:
            self.url = lx[0].text

        lx = node.xpath("./a:nodeSourceName", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 0 or len(lx) == 1)
        if len(lx) == 1:
            self.node_source_name = lx[0].text

        lx = node.xpath("./a:credential", namespaces = {'a' : main.xmlns})
        assert(len(lx) == 1 or len(lx) == 0)
        if len(lx) == 1:
            self.credential = lx[0].text

    def getClass(self):
        if self.java_starter_class is None:
            return "org.ow2.proactive.resourcemanager.utils.RMNodeStarter"
        else:
            return self.java_starter_class
    
    def getArguments(self):
        args = []

        if self.url is not None:
            args.append("-r")
            args.append(self.url)
        
        if self.nodename is not None:
            args.append("-n")
            args.append(self.nodename)

        if self.node_source_name is not None:
            args.append("-s")
            args.append(self.node_source_name)
        
        if self.credential is not None:
            args.append("-f")
            args.append(self.credential)
        
        return args


class CustomConnection(_AbstractConnection):
    ''' 
    Spawns a JVM who will run user supplied code
    '''
    def __init__(self):
        _AbstractConnection.__init__(self)
        
        self.jvmParameters = None
        self.args = []
    
    def parse(self, node):
        _AbstractConnection.parse(self, node)
        logger.debug("%s parse called" % self.__class__.__name__)
        
        lx = node.xpath("./a:args/a:arg", namespaces = {'a' : main.xmlns})
        if len(lx) == 0:
            self.jvmParameters = [] 
        for node in lx:
            self.args.append(node.text)

        assert self.java_starter_class is not None, "javaStarterClass must be defined with custom connection"

    def getClass(self):
        return self.java_starter_class
    
    def getArguments(self):
        return self.args

    
class LocalBindConnection(_AbstractConnection):
    '''
    Spawns a JVM who registers into a local registery. 
    '''
    def __init__(self):
        _AbstractConnection.__init__(self)
     
    def parse(self, node):
        _AbstractConnection.parse(self, node)
        logger.debug("%s parse called" % self.__class__.__name__)
        
        if self.java_starter_class is None:
            self.java_starter_class = "org.objectweb.proactive.core.util.winagent.PAAgentServiceRMIStarter"
        
    def getClass(self):
        return self.java_starter_class
        
    def getArguments(self):
        if self.nodename == None:
            return [_nodename.next()]
        else:
            return [self.nodename]


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
        raise errors.AgentConfigFileError('One and only one connection must be enabled (not enforced by the XML Schema)')
    elif (l == 1):
        name = lx[0].tag.replace("{%s}" % main.xmlns, "", 1)
        handler = handlers.get(name)()
        handler.parse(lx[0])
        return handler
    else:
        raise errors.AgentConfigFileError('One connection must be enabled (not enforced by the XML Schema)')
