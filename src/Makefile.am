#paagent paagent_server
#dependencies
bin_PROGRAMS = paagent paagent_server
bin_SCRIPTS = 
noinst_PROGRAMS = paagent_tests

paagent_server_SOURCES= Configuration.cpp P2PNodeStarter.cpp CustomNodeStarter.cpp RMNodeStarter.cpp  \
					    AdvertAction.cpp CustomAction.cpp RMAction.cpp P2PAction.cpp Action.cpp \
					    PAAgent.cpp paagent_server.cpp Controller.cpp NodeStarter.cpp \
					    Event.cpp CalendarEvent.cpp IdlenessEvent.cpp \
					    Event.h CalendarEvent.h IdlenessEvent.h \
					    Configuration.h P2PNodeStarter.h CustomNodeStarter.h RMNodeStarter.h  \
					    PAAgent.h Controller.h NodeStarter.h Constants.h controller_adapter.h
paagent_server_LDADD = $(GNUCXX_LIBS) $(DBUS_CXX_LIBS) $(LOG4CXX_LIBS)
paagent_server_CXXFLAGS = $(DBUS_CXX_CFLAGS) $(LOG4CXX_CFLAGS)\
				 		 $(GNUCXX_CFLAGS) -std=c++0x -std=gnu++0x

paagent_tests_SOURCES = P2PNodeStarter.cpp CustomNodeStarter.cpp RMNodeStarter.cpp  \
						Runner.cpp ConfigParserTest.cpp AdvertAction.cpp CustomAction.cpp \
						RMAction.cpp P2PAction.cpp Action.cpp Event.cpp CalendarEvent.cpp \
						IdlenessEvent.cpp Configuration.cpp ConfigParser.cpp ConfigHandler.cpp \
						paagent_tests.cpp PAAgentTest.cpp ControllerTest.cpp  PAAgent.cpp \
						Controller.cpp NodeStarter.cpp  Watcher.cpp \
						P2PNodeStarter.h CustomNodeStarter.h RMNodeStarter.h  \
						Runner.h ConfigParserTest.h AdvertAction.h CustomAction.h \
						RMAction.h P2PAction.h Action.h Event.h CalendarEvent.h \
						IdlenessEvent.h Configuration.h ConfigParser.h ConfigHandler.h \
						PAAgentTest.h ControllerTest.h  PAAgent.h controller_proxy.h \
						Controller.h NodeStarter.h  Watcher.h Constants.h XMLDefs.h
						 
paagent_tests_LDADD = -lgtest $(XERC_LIBS) $(GNUCXX_LIBS) $(DBUS_CXX_LIBS) $(LOG4CXX_LIBS) 
paagent_tests_CXXFLAGS = -I/usr/include/gtest $(DBUS_CXX_CFLAGS) $(LOG4CXX_CFLAGS)\
				 $(GNUCXX_CFLAGS) -std=c++0x -std=gnu++0x

paagent_SOURCES = P2PNodeStarter.cpp CustomNodeStarter.cpp RMNodeStarter.cpp Runner.cpp \
				  AdvertAction.cpp CustomAction.cpp RMAction.cpp P2PAction.cpp Action.cpp \
				  Event.cpp CalendarEvent.cpp IdlenessEvent.cpp Configuration.cpp \
				  ConfigParser.cpp ConfigHandler.cpp PAAgent.cpp paagent.cpp \
				  Controller.cpp NodeStarter.cpp  Watcher.cpp controller_proxy.h \
				  P2PNodeStarter.h CustomNodeStarter.h RMNodeStarter.h Runner.h \
				  AdvertAction.h CustomAction.h RMAction.h P2PAction.h Action.h \
				  Event.h CalendarEvent.h IdlenessEvent.h Configuration.h \
				  ConfigParser.h ConfigHandler.h PAAgent.h \
				  Controller.h NodeStarter.h  Watcher.h Constants.h XMLDefs.h
				  
paagent_LDADD = $(XERC_LIBS)  $(GNUCXX_LIBS) $(DBUS_CXX_LIBS) $(LOG4CXX_LIBS)
paagent_CXXFLAGS = $(DBUS_CXX_CFLAGS) $(LOG4CXX_CFLAGS)\
				 $(XERC_CFLAGS) $(GNUCXX_CFLAGS) -std=c++0x -std=gnu++0x


#dbus-cxx-xml2cpp --xml Controller.xml --adapter -f
controller-server-bindings: $(interface_xml)
	dbus-cxx-xml2cpp --xml $(interface_xml) --adapter -f 

controller-client-bindings: $(interface_xml)
	dbus-cxx-xml2cpp --xml $(interface_xml) --proxy -f 

bindings: controller-server-bindings controller-client-bindings
	
# Interface XML name 
interface_xml = Controller.xml

# Target for DTD validation on the interface XML. 
# Run manually because it needs xmllint and Net conectivity
checkxml: $(interface_xml)
	@xmllint --valid --noout $<
	@echo $< checks out ok

	
