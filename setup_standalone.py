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

# Check for JAVA_HOME needed to embed jre
try:  
   javaHome = os.environ['JAVA_HOME']
   print("Using JAVA_HOME=" + javaHome)
except KeyError: 
   print "Please set the environment variable JAVA_HOME"
   sys.exit(1)

# Check for jre
systemJreDir = javaHome+"/jre"
if not os.path.exists(systemJreDir):
   print "Unable to locate the system jre, JAVA_HOME must be pointing to a jdk"
   sys.exit(1)

# Check for SCHEDULER_HOME needed to embed schedworker
try:
   schedDir = os.environ['SCHEDULER_HOME']
   print("Using SCHEDULER_HOME=" + schedDir)
except KeyError: 
   print "Please set the environment variable SCHEDULER_HOME"
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
				("README.txt", "README.txt"),
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

def copyfile(filename, fromDir, toDir):
    if os.path.exists(fromDir+"/"+filename):
        if not os.path.exists(toDir):
            mkdir_p(toDir)
        shutil.copy(fromDir+"/"+filename, toDir+"/"+filename)

arch = ("amd64" if (sys.maxsize > 2**32) else "i386")

# Copy the required files and dirs into the schedworker
copyfile("LICENSE.txt", schedDir, workerDir)
copyfile("LICENSE_EXCEPTION.txt", schedDir, workerDir)
copyfile("README.txt", schedDir, workerDir)
copyfile("build_id", schedDir, workerDir)

unixDir = "/bin/unix"
configDir = "/config"
authDir = configDir+"/authentication"
log4jDir = configDir+"/log4j"
proactiveDir = configDir+"/proactive"
libDir = "/dist/lib"
linuxDir = "/dist/scripts/processbuilder/linux"

copyfile("env", schedDir+unixDir, workerDir+unixDir)
copyfile("rm-start-node", schedDir+unixDir, workerDir+unixDir)
copyfile("security.java.policy-client", schedDir+configDir, workerDir+configDir)
copyfile("rm.cred", schedDir+authDir, workerDir+authDir)
copyfile("log4j-defaultNode", schedDir+log4jDir, workerDir+log4jDir)
copyfile("ProActiveConfiguration.xml", schedDir+proactiveDir, workerDir+proactiveDir)

distutils.dir_util.copy_tree(schedDir+libDir, workerDir+libDir)
# fix agent bug: the classpath is not read recursively it means contents of dist/lib/sigar dir are not added to classpath
shutil.rmtree(workerDir+libDir+"/sigar")
copyfile("sigar.jar", schedDir+libDir+"/sigar", workerDir+libDir)
if arch == "amd64":
	copyfile("libsigar-amd64-linux.so", schedDir+libDir+"/sigar", workerDir+libDir)
else:
	copyfile("libsigar-x86-linux.so", schedDir+libDir+"/sigar", workerDir+libDir)

if os.path.exists(schedDir+linuxDir):
    distutils.dir_util.copy_tree(schedDir+linuxDir, workerDir+linuxDir)	

# Embed jre
jreDir = packagingDir+"/jre"
mkdir_p(jreDir)
distutils.dir_util.copy_tree(systemJreDir, jreDir)

# Remove the following optional jars (see jre1.7 README)
os.remove(jreDir+"/lib/ext/dnsns.jar")
os.remove(jreDir+"/lib/ext/localedata.jar")
os.remove(jreDir+"/lib/ext/sunec.jar")
os.remove(jreDir+"/lib/ext/sunjce_provider.jar")
os.remove(jreDir+"/lib/ext/sunpkcs11.jar")

os.remove(jreDir+"/lib/javaws.jar")
os.remove(jreDir+"/lib/deploy.jar")
os.remove(jreDir+"/lib/plugin.jar")

shutil.rmtree(jreDir+"/lib/deploy")

os.remove(jreDir+"/bin/rmid")
os.remove(jreDir+"/bin/rmiregistry")
os.remove(jreDir+"/bin/servertool")
os.remove(jreDir+"/bin/tnameserv")
os.remove(jreDir+"/bin/orbd")
os.remove(jreDir+"/bin/keytool")
os.remove(jreDir+"/bin/javaws")

# Use pack200 to compress rt.jar
print("Packing "+jreDir+"/lib/rt.jar please wait ...")
cmd = [jreDir+"/bin/pack200","-J-Xmx1024m", jreDir+"/lib/rt.jar.pack.gz", jreDir+"/lib/rt.jar"]
p = subprocess.Popen(cmd, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
(stdout, stderr) = p.communicate()
print stdout
print >> sys.stderr, stderr
if p.returncode != 0:
   raise Exception("A problem occured during rt.jar packing")

# Delete rt.jar
os.remove(jreDir+"/lib/rt.jar")

#print("Contents of " + workerDir + ":")
#for path,dirs,files in os.walk(workerDir):
#    for fn in files:
#        print(os.path.join(path,fn))

packageName = "proactive-agent-"+ver+"-"+arch+"-standalone.deb"

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
# controlFile.write("Recommends: oracle-java6-installer"+os.linesep)
controlFile.close()

# Create the .deb package
p = subprocess.Popen(["dpkg","-b", "./packaging/debian", packageName], stdin=subprocess.PIPE, stderr=subprocess.PIPE)
(stdout, stderr) = p.communicate()
print stdout
print >> sys.stderr, stderr
if p.returncode != 0:
   raise Exception("A problem occured during .deb generation")

