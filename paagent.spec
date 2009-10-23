Summary: paagent                     
Name: paagent                        
Version: 0.1.4
Release: 1
Source0: %{name}-%{version}.tar.gz
License: GPL
Group: Development/Tools
BuildRoot: %{_builddir}/%{name}-root
%description
An agent for starting ProActive JVM nodes at specified
times on a computer.

%prep
%setup -q

%build
./configure
make

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install_services_fedora

%post
chkconfig --add paas
chkconfig --level 345 paas  on
chkconfig --level 016 paas  off
chkconfig --add paac
chkconfig --level 345 paac  on
chkconfig --level 016 paac  off

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/local/bin/paagent
/usr/local/bin/paagent_server
/etc/paagent/config.xml
/etc/paagent/server-log4cxx
/etc/paagent/client-log4cxx
/var/log/paagent
/etc/paagent/PAAgent-config-planning-day-only.xml
/etc/paagent/PAAgent-config-planning-night-we.xml
/etc/rc.d/init.d/paac
/etc/rc.d/init.d/paas
