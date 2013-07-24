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

import sys, os, errno, platform, shutil, subprocess, distutils.dir_util, math
from cx_Freeze import setup, Executable

# Version to build
ver = "1.0.3"
print("Building ProActive Agent standalone v" + ver)

# Try to read the env variable needed for the embedded schedworker
try:  
   schedDir = os.environ['SCHEDULER_HOME']
   print("Using SCHEDULER_HOME=" + schedDir)
except KeyError: 
   print "Please set the environment variable SCHEUDLER_HOME"
   sys.exit(1)

print("Freezing (building binaries from sources) ...")

# Native libs that will be embedded by the freezed standalone
includes = ["lxml", "lxml._elementpath", "lxml.etree", "gzip"]
res = setup(
      name = "ProActive-LinuxAgent",
      version = ver,
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

# Get cx_freeze build output dir
freezedDir = os.listdir("./build")[0]

# Copy the generated standalone binaries into packaging dir
packagingDir = "./packaging/debian/opt/proactive-agent"
distutils.dir_util.copy_tree("./build/"+freezedDir, packagingDir)

# Create the schedworker dir
workerDir = packagingDir+"/schedworker"
if not os.path.exists(workerDir):
    os.makedirs(workerDir)

# No error will be reported if a directory given as an operand already exists
def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc: # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else: raise

# Copy the required files and dirs into the schedworker
if os.path.exists(schedDir+"/LICENSE.txt"):
    shutil.copy(schedDir+"/LICENSE.txt", workerDir+"/LICENSE.txt")
if os.path.exists(schedDir+"/LICENSE_EXCEPTION.txt"):
    shutil.copy(schedDir+"/LICENSE_EXCEPTION.txt", workerDir+"/LICENSE_EXCEPTION.txt")
if os.path.exists(schedDir+"/README.txt"):
    shutil.copy(schedDir+"/README.txt", workerDir+"/README.txt")
if os.path.exists(schedDir+"/build_id"):
    shutil.copy(schedDir+"/build_id", workerDir+"/build_id")
	
unixDir = "/bin/unix"
configDir = "/config"
authDir = configDir+"/authentication"
log4jDir = configDir+"/log4j"
proactiveDir = configDir+"/proactive"
libDir = "/dist/lib"
linuxDir = "/dist/scripts/processbuilder/linux"

mkdir_p(workerDir+unixDir)
mkdir_p(workerDir+configDir)
mkdir_p(workerDir+authDir)
mkdir_p(workerDir+log4jDir)
mkdir_p(workerDir+proactiveDir)
mkdir_p(workerDir+libDir)
mkdir_p(workerDir+linuxDir)

shutil.copy(schedDir+unixDir+"/env", workerDir+unixDir+"/env")
shutil.copy(schedDir+unixDir+"/rm-start-node", workerDir+unixDir+"/rm-start-node")
shutil.copy(schedDir+configDir+"/security.java.policy-client", workerDir+configDir+"/security.java.policy-client")
shutil.copy(schedDir+authDir+"/rm.cred", workerDir+authDir+"/rm.cred")
shutil.copy(schedDir+log4jDir+"/log4j-defaultNode", workerDir+log4jDir+"/log4j-defaultNode")
shutil.copy(schedDir+proactiveDir+"/ProActiveConfiguration.xml", workerDir+proactiveDir+"/ProActiveConfiguration.xml")

distutils.dir_util.copy_tree(schedDir+libDir, workerDir+libDir)
distutils.dir_util.copy_tree(schedDir+linuxDir, workerDir+linuxDir)

#print("Contents of " + workerDir + ":")
#for path,dirs,files in os.walk(workerDir):
#    for fn in files:
#        print(os.path.join(path,fn))

arch = "x86" + ("-64" if (sys.maxsize > 2**32) else "") 
packageName = "proactive-agent-"+ver+"-"+arch+".deb"

def get_size(start_path = '.'):
    total_size = 0
    for dirpath, dirnames, filenames in os.walk(start_path):
        for f in filenames:
            fp = os.path.join(dirpath, f)
            total_size += os.path.getsize(fp)
    return total_size

def convertSize(size):
   size_name = ("bytes", "KB", "MB", "GB")
   i = int(math.floor(math.log(size,1024)))
   p = math.pow(1024,i)
   s = round(size/p,2)
   if (s > 0):
       return '%s %s' % (s,size_name[i])
   else:
       return '0B'

installedSize = get_size(packagingDir)
print("Building debian package "+packageName+" estimated installed size: "+convertSize(installedSize)+" ...")

# Write Version and Architecture into control file
controlFile = open("./packaging/debian/DEBIAN/control", 'w')
controlFile.write("Package: proactive-agent"+os.linesep)
controlFile.write("Version: "+ver+os.linesep)
controlFile.write("Architecture: "+arch+os.linesep)
controlFile.write("Maintainer: activeeon-dev@activeeon.com"+os.linesep)
controlFile.write("Priority: optional"+os.linesep)
controlFile.write("Installed-Size: "+str(installedSize)+os.linesep)
controlFile.write("Description: The ProActive Agent enables desktop computers as an important source of computational power"+os.linesep)
controlFile.write("Recommends: oracle-java6-installer"+os.linesep)
controlFile.close()

# Create the .deb package
p = subprocess.Popen(["dpkg","-b", "./packaging/debian", packageName], stdin=subprocess.PIPE, stderr=subprocess.PIPE)
(stdout, stderr) = p.communicate()
print stdout
print >> sys.stderr, stderr
if p.returncode != 0:
   raise Exception("A problem occured during .deb generation")

