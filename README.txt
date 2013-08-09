ProActive Agent is a system daemon to automatically starts ProActive runtimes according to a weekly schedule

#################################
## How to change configuration ##
#################################

Note: config.xml is a symbolic link that points by default to config-fulltime.xml

After changing the configuration files, restart the proactive-agent daemon
> sudo /etc/init.d/proactive-agent restart

###################################
## Uninstall the proactive-agent ##
###################################

sudo dpkg -r proactive-agent

##################
## Known issues ##
##################

---- Missing libxslt1.1 ----
Symptom: At the end of the installation appears the following message:
> lxml not available: libxslt.so.1: cannot open shared object file: No such file or directory (please check your installation)
Solution: Install libxslt1.1
> sudo apt-get install libxslt1.1

---
Copyright (C) 2007-2013 ActiveEon
Visit http://proactive.inria.fr/ and http://www.activeeon.com/
Contact: +33 (0)9 88 777 660, contact@activeeon.com 

