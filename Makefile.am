#Makefile: makefile for paagent.cpp
SUBDIRS = src 
EXTRA_DIST = src/PAAgent-config-planning-night-we.xml src/paac src/paas \
			src/paagent-log4cxx src/PAAgent-config.xml src/Controller.xml \
			src/PAAgent-config-planning-day-only.xml Doxyfile images \
			src/client-log4cxx src/server-log4cxx src/paas_fedora src/paac_fedora \
			paagent.spec

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
install_services_fedora: install 
	mkdir -p $(DESTDIR)/etc/init.d
	mkdir -p $(DESTDIR)/etc/paagent
	mkdir -p $(DESTDIR)/var/log/paagent
	cp src/PAAgent-config-planning-day-only.xml $(DESTDIR)/etc/paagent/ 
	cp src/PAAgent-config-planning-night-we.xml $(DESTDIR)/etc/paagent/
	cp src/PAAgent-config.xml $(DESTDIR)/etc/paagent/config.xml
	cp src/server-log4cxx $(DESTDIR)/etc/paagent/
	cp src/client-log4cxx $(DESTDIR)/etc/paagent/
	mkdir -p $(DESTDIR)/etc/rc.d/init.d/
	cp src/paas_fedora $(DESTDIR)/etc/rc.d/init.d/paas
	chmod +x $(DESTDIR)/etc/rc.d/init.d/paas
	cp src/paac_fedora $(DESTDIR)/etc/rc.d/init.d/paac
	chmod +x $(DESTDIR)/etc/rc.d/init.d/paac
	
	
#delete log and configuration folders
#remove installed system services
#delete init.d scripts
remove_services_fedora: uninstall
	rm -rf $(DESTDIR)/etc/paagent
	rm -rf $(DESTDIR)/var/log/paagent
	chkconfig --del paac
	chkconfig --del paas
	rm -f $(DESTDIR)/etc/rc.d/init.d/paas
	rm -f $(DESTDIR)/etc/rc.d//init.d/paac
	
	
install_services_ubuntu: install 
	mkdir -p $(DESTDIR)/etc/init.d
	mkdir -p $(DESTDIR)/etc/paagent
	mkdir -p $(DESTDIR)/var/log/paagent
	cp src/PAAgent-config-planning-day-only.xml $(DESTDIR)/etc/paagent/ 
	cp src/PAAgent-config-planning-night-we.xml $(DESTDIR)/etc/paagent/
	cp src/PAAgent-config.xml $(DESTDIR)/etc/paagent/config.xml
	cp src/server-log4cxx $(DESTDIR)/etc/paagent/
	cp src/client-log4cxx $(DESTDIR)/etc/paagent/
	cp src/paas $(DESTDIR)/etc/init.d
	chmod +x $(DESTDIR)/etc/init.d/paas
	cp src/paac $(DESTDIR)/etc/init.d
	chmod +x $(DESTDIR)/etc/init.d/paac
	
	
#delete log and configuration folders
#remove installed system services
#delete init.d scripts
remove_services_ubuntu: uninstall
	rm -rf $(DESTDIR)/etc/paagent
	rm -rf $(DESTDIR)/var/log/paagent
	rm -f $(DESTDIR)/etc/init.d/paas
	rm -f $(DESTDIR)/etc/init.d/paac	
	
	
	
#backup .rpmmacros
#create rpmbuild tree rpmdev-setuptree
#create paagent dist package	
#copy paagent dist package to SOURCES
#set $RPM_BUILD_ROOT
#run rpbmbuild -ba paagent.spec
#copy built packages to somewhere visible
#restore .rpmmacros
rpm: dist
	mv ~/.rpmmacros ~/.rpmmacros_paagent_bkp
	echo "%_topdir %(pwd)/rpmbuild" > ~/.rpmmacros
	mkdir -p rpmbuild
	mkdir -p rpmbuild/RPMS rpmbuild/SOURCES rpmbuild/SPECS rpmbuild/SRPMS rpmbuild/BUILD
	cp $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz rpmbuild/SOURCES
	rpmbuild -ba paagent.spec
	mv ~/.rpmmacros_paagent_bkp ~/.rpmmacros
	

	
#create debuild folder
#create paagent dist package	
#copy and rename paagent dist packages
#unpack the dist package
#copy the debian settings 
#run debuild 
DEBUILD=debuild
deb: dist
	rm -rf $(DEBUILD)
	mkdir -p $(DEBUILD)
	tar xvf $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz
	mv $(PACKAGE_NAME)-$(PACKAGE_VERSION) $(DEBUILD)
	cp $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz debuild/$(PACKAGE_NAME)_$(PACKAGE_VERSION).orig.tar.gz
	cp -r debian $(DEBUILD)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)
	rm -rf $(DEBUILD)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)/debian/.svn
	cd $(DEBUILD)/$(PACKAGE_NAME)-$(PACKAGE_VERSION); debuild