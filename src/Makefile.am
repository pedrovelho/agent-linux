#paagent paagent_server

bin_PROGRAMS = paagent paagent_server
bin_SCRIPTS = 
noinst_PROGRAMS = paagent_tests

paagent_server_SOURCES= P2PNodeStarter.cpp CustomNodeStarter.cpp RMNodeStarter.cpp  \
					    PAAgent.cpp paagent_server.cpp Controller.cpp NodeStarter.cpp \
					    P2PNodeStarter.h CustomNodeStarter.h RMNodeStarter.h  \
					    PAAgent.h Controller.h NodeStarter.h Constants.h controller_adapter.h
paagent_server_LDADD = -llog4cxx -lccext2 -lccgnu2 -lrt \
                       -ldl -lsigc-2.0 -ldbus-cxx
paagent_server_LDFLAGS = -pthread  -D_GNU_SOURCE 
paagent_server_CXXFLAGS = -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include \
						  -I/usr/local/include/dbus-cxx-0.4 -I/usr/include/sigc++-2.0 \
						  -I/usr/lib/sigc++-2.0/include -std=c++0x -std=gnu++0x

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
						 
paagent_tests_LDADD = -lgtest -lxerces-c-3.0 -llog4cxx  -lccgnu2   -lrt \
					  -ldl -lsigc-2.0 -ldbus-cxx
paagent_tests_LDFLAGS = -pthread -D_GNU_SOURCE 
paagent_tests_CXXFLAGS = -I/usr/include/gtest -I/usr/include/sigc++-2.0 \
                         -I/usr/lib/sigc++-2.0/include -I/usr/local/include/dbus-cxx-0.4 \
                         -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include \
                         -std=c++0x -std=gnu++0x

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
				  
paagent_LDADD = -lxerces-c-3.0 -llog4cxx  -lccgnu2 -lrt \
				-ldl -lsigc-2.0 -ldbus-cxx
paagent_LDFLAGS = -pthread  -D_GNU_SOURCE 
paagent_CXXFLAGS = -I/usr/include/tclap -I/usr/include/sigc++-2.0 \
				   -I/usr/lib/sigc++-2.0/include -I/usr/local/include/dbus-cxx-0.4 \
				   -I/usr/include/sigc++-2.0 \
				   -I/usr/lib/sigc++-2.0/include -std=c++0x -std=gnu++0x


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
