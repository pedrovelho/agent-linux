ProActive Agent is a set of scripts to automatically starts ProActive Nodes according to a weekly schedule

Typically you should configure

bin/config
bin/node

to tune the number of nodes you'd like to run on a computer and proactive credentials, urls etc.

It also a good practice to start nodes under a dedicated user account

$ sudo groupadd proactive
$ sudo useradd -m -g proactive proactive
# change the password of newly created user
$ sudo passwd proactive
# switch to this account
$ sudo su - proactive

---
Copyright (C) 2007-2011 ActiveEon
Visit http://proactive.inria.fr/ and http://www.activeeon.com/
Contact: +33 (0)9 88 777 660, contact@activeeon.com 

