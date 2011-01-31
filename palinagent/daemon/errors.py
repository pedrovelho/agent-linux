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


class AgentError(Exception):
    '''
    Base class exception for the ProActive Linux agent
    '''
    pass

class AgentInternalError(AgentError):
    '''
    ProActive Linux agent internal error
    
    A such exception is raised when the agent is in an unexpected or bad state. If a
    such exception is encountered it means that you do a bug report ;)
    '''
    def __init(self):
        self.message = message

    def __str__(self):
        return "%s (This is an internal error, please fill a bug report)" % self.message
    
        
class AgentSetupError(AgentError):
    def __init__(self, message):
        self.message = message

    def __str__(self):
        return "%s (please check your installation)" % self.message
    

class AgentConfigFileError(AgentError):
    def __init__(self, message):
        self.message = message

    def __str__(self):
        return  "Invalid configuration file: %s" % self.message
