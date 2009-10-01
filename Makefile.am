#Makefile: makefile for paagent.cpp
SUBDIRS = src 
EXTRA_DIST = src/PAAgent-config-planning-night-we.xml src/paac src/paas \
			src/paagent-log4cxx src/PAAgent-config.xml src/Controller.xml \
			src/PAAgent-config-planning-day-only.xml Doxyfile images \
			src/client-log4cxx src/server-log4cxx

doc: 
	doxygen Doxyfile

clean_doc:
	rm -rf ./doc

#create config dir in /etc/paagent 
#create log dir in /var/log 
#copy config files in /etc/paagent
#copy scripts
#make sure they are executable
#install as sys service
#start with lowest priority in runlevel 3 and stop before all in shutdown, reboot and single
install_services: install 
	mkdir -p /etc/paagent
	mkdir -p /var/log/paagent
	cp src/PAAgent-config-planning-day-only.xml /etc/paagent/ 
	cp src/PAAgent-config-planning-night-we.xml /etc/paagent/
	cp src/PAAgent-config.xml /etc/paagent/config.xml
	cp src/server-log4cxx /etc/paagent/
	cp src/client-log4cxx /etc/paagent/
	cp src/paas /etc/init.d
	chmod +x /etc/init.d/paas
	update-rc.d paas start 98 3 4 5 . stop 2 0 1 6 .
	cp src/paac /etc/init.d
	chmod +x /etc/init.d/paac
	update-rc.d paac start 99 3 4 5 . stop 1 0 1 6 .
	
#delete log and configuration folders
#remove installed system services
#delete init.d scripts
remove_services: uninstall
	rm -rf /etc/paagent
	rm -rf /var/log/paagent
	update-rc.d -f paac remove
	update-rc.d -f paas remove
	rm -f /etc/init.d/paas
	rm -f /etc/init.d/paac