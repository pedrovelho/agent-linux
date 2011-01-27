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

from setuptools import setup, find_packages
from palinagent.daemon.main  import version   as agent_version
import palinagent.daemon.tests.testsuite 

version = agent_version

setup(
      name="ProActive-LinuxAgent",
      version=version,
      description="A daemon to automatically start ProActive runtime according to a schedule",
      long_description=open("README.txt").read(),
      classifiers=[
        "Development Status :: Development Status :: 4 - Beta",
        "Environment :: No Input/Output (Daemon",
        "Environment :: Console",
        "Intended Audience :: System Administrators",
        "Intended Audience :: Science/Research",
        "Intended Audience :: Financial and Insurance Industry",
        "Intended Audience :: Information Technology",
        "License :: OSI Approved :: GNU Affero General Public License (AGPL)"
        "Operating System :: POSIX",
        "Operating System :: Unix",
        "Programming Language :: Python",
        "Topic :: System :: Distributed Computing",
        "Topic :: System :: Systems Administration",
        ],
      keywords="ProActive",
      author="Activeeon",
      author_email="contact@activeeon.com",
      url="http://activeeon.com",
      license="AGPL",


      
      zip_safe=False,
      include_package_data=True,

      namespace_packages=["palinagent"],
      packages=find_packages(),

      entry_points= {
        'console_scripts': [
          'proactive-agent = palinagent.daemon.main:main_func',
          ],
        },
        
      test_suite = "palinagent.daemon.tests.testsuite.get_testsuite",
      scripts = ["build_lxml", "fix_lxml"],
      data_files = ["AUTHORS.txt", "LICENSE.txt", "README.txt"]
      )

