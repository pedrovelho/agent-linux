'''
Created on Jul 7, 2010

@author: cmathieu
'''

import logging
from lxml import etree

logger = logging.getLogger("action")

class __RootAction:
    def getStart(self, config):
        return lambda :self._start(config)
    def getStop(self, config):
        return lambda :self._stop(config)
    def getRestart(self, config):
        return lambda :self._restart(config)
    def parse(self, node):
        raise NotImplementedError
                    
    def _start(self, config):
        raise NotImplementedError

    def _stop(self, config):
        raise NotImplementedError

    def _restart(self, config):
        raise NotImplementedError


class Dummy(__RootAction):

    def parse(self, doc):
        logger.debug("%s parse called" % self.__class__.__name__)
        return "/bin/true"

    def _start(self, config):
        logger.debug("%s start called" % self.__class__.__name__)
        return "/bin/true"

    def _stop(self, config):
        logger.debug("%s stop called" % self.__class__.__name__)
        return "/bin/true"

    def _restart(self, config):
        logger.debug("%s restart called" % self.__class__.__name__)
        return "/bin/true"
  
    
class Advert(__RootAction):
    __nodeName = None
    __javaStarterClass = None
    __initialRestartDelay = None 
     
    def parse(self, node):
        logger.debug("%s parse called" % self.__class__.__name__)
        
        lx = node.xpath("./a:nodeName", namespaces = {'a' : "urn:proactive:agent:1.0"});
        assert(len(lx) == 1)
        self.__nodeName = lx[0].text

        lx = node.xpath("./a:javaStarterClass", namespaces = {'a' : "urn:proactive:agent:1.0"});
        assert(len(lx) == 1)
        self.__javaStarterClass = lx[0].text

        lx = node.xpath("./a:initialRestartDelay", namespaces = {'a' : "urn:proactive:agent:1.0"});
        assert(len(lx) == 1)
        self.__initialRestartDelay = lx[0].text
        
        assert(self.__nodeName is not None)
        assert(self.__javaStarterClass is not None)
        assert(self.__nodeName is not None)
        
        return self
    
    def _start(self, config):
        logger.debug("%s start called" % self.__class__.__name__)
        cmd = _buildJavaCommand(config, self.__javaStarterClass, self.__nodeName);
        return cmd
    
    def _stop(self, config):
        logger.debug("%s stop called" % self.__class__.__name__)
        return "/bin/true"

    def _restart(self, config):
        logger.debug("%s restart called" % self.__class__.__name__)
        return "/bin/true"

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
    
_handlers = {'AdvertAction' : Advert}

def parse(filename):
    tree = etree.parse(filename)
    lx = tree.xpath("//a:action[a:isEnabled='true' or a:isEnabled='1']", namespaces = {'a' : "urn:proactive:agent:1.0"})

    l = len(lx)
    if (l == 0):
        return Dummy.parse()
    elif (l == 1):
        name = lx[0].get("type")
        handler = _handlers.get(name)()
        return handler.parse(lx[0])
    else:
        raise Exception('Invalid config file. One and only one action can be enabled in the config file (it is allowed by the XSD but not by the daemon)')
