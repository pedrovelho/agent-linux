ProActive Agent is a system daemon to automatically starts ProActive runtimes according to a weekly schedule

################################################
## HOW TO RUN TESTS                           ##
################################################

1) Install python2.7:
> sudo apt-get install python2.7 python2.7-dev python-setuptools

2) Install libxml2 and xmllint:
> sudo apt-get install libxml2-dev libxslt-dev libxml2-utils

3) Requires lxml:
> sudo easy_install-2.7 lxml==3.2.1

4) Install coverage:
> sudo easy_install-2.7 coverage

5) Export current dir:
> export PYTHONPATH=.

6) Run tests:
> python2.7 palinagent/daemon/tests/testsuite.py -c xml -o xml

!! Check the xml output file !!

If there is an "ImportError No module named" used dos2unix:
> sudo apt-get install dos2unix
> find . -type f -exec dos2unix {} \;

Delete all .pyc files
> find . -name "*.pyc" -exec rm -rf {} \;

################################################
## HOW TO GENERATE BINARIES in x86 and x86_64 ##
################################################

To generate the binaries that you see in directories x86 and x86_64 (used as main
ingredient to generate the distributables) please proceed as follows. 

1) Install cx_freeze:
> sudo easy_install-2.7 cx_freeze

2) Export a JAVA_HOME pointing to a jdk >=1.6
> export JAVA_HOME=/user/proactive/jdk/jdk1.7.0_25

3) Download Scheduling release and export SCHEDULER_HOME (tested with 3.3.2 and 3.4):
> export SCHEDULER_HOME=/user/proactive/releases/ProActiveScheduling-3.4.0_bin_full

4) Run the build script:
> python2.7 setup_standalone.py build

There should be a .deb generated in the current dir
Example of output:
> ...
> Building debian package proactive-agent-1.0.3-i386-standalone.deb estimated installed size: 166.25 MB ...
> dpkg-deb: building package `proactive-agent' in `proactive-agent-1.0.3-x86-64.deb'.
> None

If your get an error ending with:
> cx_Freeze.freezer.ConfigError: no initscript named Console

You need to reinstall cx_freeze:
Download cx_freeze from sources: http://prdownloads.sourceforge.net/cx-freeze/cx_Freeze-4.3.1.tar.gz?download
Remove previous installation with pip:
> sudo apt-get install python-pip
> sudo pip-2.7 uninstall cx_Freeze
> sudo setup.py install

---
Copyright (C) 2007-2013 ActiveEon
Visit http://proactive.inria.fr/ and http://www.activeeon.com/
Contact: +33 (0)9 88 777 660, contact@activeeon.com 

