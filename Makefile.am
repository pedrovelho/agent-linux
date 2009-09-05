#Makefile: makefile for paagent.cpp
 
#paagent
bin_PROGRAMS=paagent_server paagent

paagent_server_SOURCES=PAAgent.cpp paagent_server.cpp Controller.cpp NodeStarter.cpp  Watcher.cpp 
paagent_server_LDADD = -llog4cxx -lccext2 -lccgnu2 -lz -ldl -lrt -ldbus-1 -lgthread-2.0  -ldbus-glib-1 -lglibmm-2.4  -lsigc-2.0 -ldbus-cxx
paagent_server_LDFLAGS = -pthread   -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -D_GNU_SOURCE 
paagent_server_CXXFLAGS = -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/local/include/dbus-cxx-0.4 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -std=c++0x -std=gnu++0x


paagent_tests_SOURCES = paagent_tests.cpp PAAgentTest.cpp ControllerTest.cpp  PAAgent.cpp paagent.cpp Controller.cpp NodeStarter.cpp  Watcher.cpp DBusMessaging.cpp
paagent_tests_LDADD = -lgtest -llog4cxx -lccext2 -lccgnu2 -lz -ldl -lrt -ldbus-1 -lgthread-2.0  -ldbus-glib-1 -lglibmm-2.4  -lsigc-2.0 -ldbus-cxx
paagent_tests_LDFLAGS = -pthread   -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -D_GNU_SOURCE 
paagent_tests_CXXFLAGS = -I/usr/include/gtest -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/local/include/dbus-cxx-0.4 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -std=c++0x -std=gnu++0x

paagent_SOURCES = AdvertAction.cpp CustomAction.cpp RMAction.cpp P2PAction.cpp Action.cpp Event.cpp CalendarEvent.cpp IdlenessEvent.cpp Configuration.cpp ConfigParser.cpp ConfigHandler.cpp PAAgent.cpp paagent.cpp Controller.cpp NodeStarter.cpp  Watcher.cpp DBusMessaging.cpp
paagent_LDADD = -lxerces-c-3.0 -llog4cxx -lccext2 -lccgnu2 -lz -ldl -lrt -ldbus-1 -lgthread-2.0  -ldbus-glib-1 -lglibmm-2.4  -lsigc-2.0 -ldbus-cxx
paagent_LDFLAGS = -pthread   -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -D_GNU_SOURCE 
paagent_CXXFLAGS = -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/local/include/dbus-cxx-0.4 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -std=c++0x -std=gnu++0x

#dbus-cxx-xml2cpp --xml Controller.xml --adapter -f
controller-server-bindings: $(interface_xml)
	dbus-cxx-xml2cpp --xml Controller.xml --adapter -f 

controller-client-bindings: $(interface_xml)
	dbus-cxx-xml2cpp --xml Controller.xml --proxy -f 

bindings: controller-server-bindings controller-client-bindings
	
# Interface XML name 
interface_xml = Controller.xml

# Target for DTD validation on the interface XML. 
# Run manually because it needs xmllint and Net conectivity
checkxml: $(interface_xml)
	@xmllint --valid --noout $<
	@echo $< checks out ok
