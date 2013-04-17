ProActive Agent - Standalone version

### configure 

  To configure and run the agent for the first time just do as follows: 

    $ export JAVA_HOME=/path/to/java
    $ sudo /opt/proactive-agent/start-agent "$JAVA_HOME"
  
  This script will configure the environment so that ProActive Agent executes 
  correctly, and then launch the latter. Once configured, the script will automatically 
  run the agent using the settings already established without asking for more 
  configuration. Erase the file SKIPCONFIGURATION to force the configuration again.

  By default a new user "proactive" will be created. The agent will be executed by
  this user, as a daemon. Log files created by the agent are readable by "proactive"
  user and by members of the "proactivegroup" group (which is also created). The 
  user who runs the above command will be optionally added to this new group, so that
  it will be able to read the agent's generated logs and files.

  At the beginning of the script file there is a list of configuration parameters 
  that can be customized.

  Immediatelly after the execution of the script, the preceding user may have been 
  added to the proactive group. Please log in again with: 

    $ su USER 

  to be able to navigate correctly the agent's directory and its content. 

### start (already configured)

    $ sudo /opt/proactive-agent/start-agent

### stop 

    $ sudo /opt/proactive-agent/stop-agent

### uninstall

    $ sudo /opt/proactive-agent/uninstall-agent.sh

### tests
  
  The standalone version was tested on the following Linux distributions: 
    - Ubuntu 7.1 
    - Ubuntu 12.10 
    - OpenSUSE 12.1 
    - Fedora 17 

