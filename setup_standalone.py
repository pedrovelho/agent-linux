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
#################################################################

from cx_Freeze import setup, Executable
 
includes = ["lxml", "lxml._elementpath", "lxml.etree", "gzip"]
setup(
      name = "ProActive-LinuxAgent",
      version = "1.0.3",
      description="A daemon to automatically start ProActive runtime according to a schedule",
      options = {
       "build_exe" : {
           "includes": includes,
            #"create_shared_zip": "False",
            #"compressed": "False",
            "include_files": [
                ("palinagent/daemon/xsd/1.0/agent-common.xsd", "xsd/1.0/agent-common.xsd"),
                ("palinagent/daemon/xsd/1.0/agent-linux.xsd", "xsd/1.0/agent-linux.xsd"),
                ("palinagent/daemon/data/agent.java.policy", "data/agent.java.policy"),
		        ("config.xml", "config.xml"),
		        ("config-fulltime.xml", "config-fulltime.xml"),
		        ("config-day.xml", "config-day.xml"),
		        ("config-night-we.xml", "config-night-we.xml"),
                #("palinagent/daemon/data/loggers.ini", "data/loggers.ini"),
                ],
            "replace_paths": [
                ("*", "<paagent_python_installation_path>/"),
                ],
           },
       },
      executables = [Executable(
          script = "proactive-agent",
          #targetName = "proactive-agent-bin",
          #appendScriptToLibrary = False,
          #appendScriptToExe = False,
          #copyDependentFiles = False,
          #compress = True,
          #silent = True
          )]
)
