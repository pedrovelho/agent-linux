%{!?python_sitelib: %define python_sitelib %(python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()")}

%if 0%{?fedora} >= 9 || 0%{?rhel} >= 6
%define with_egg 1
%else
%define with_egg 0
%endif

 
Summary: ProActive Agent
Name: proactive-agent
Version: 1.1.0-dev
Release: 1%{?dist}
License: AGPLv3
Group: System Environment/Base
Source0: http://www.activeeon.com/public_content/releases/ProActive/AgentLinux/1.0/ProActive-LinuxAgent-%{version}.tar.gz
URL: http://activeeon.com
BuildArch: noarch
BuildRequires: python

Requires: python >= 2.4
Requires: python-lxml
Requires: libcgroup

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
The ProActive Agent is a daemon process allowing to start ProActive 
resources according to a weekly sechule.

%prep
%setup -q -n ProActive-LinuxAgent-%{version}

%build
python setup.py build

%install
rm -rf $RPM_BUILD_ROOT
python setup.py install -O1 --root=$RPM_BUILD_ROOT

install -d $RPM_BUILD_ROOT/etc/rc.d/init.d
install -m 755 redhat/proactive-agent.init      $RPM_BUILD_ROOT/etc/rc.d/init.d/proactive-agent
install -d $RPM_BUILD_ROOT%{_sysconfdir}/sysconfig
install -m 644 redhat/proactive-agent.sysconfig $RPM_BUILD_ROOT%{_sysconfdir}/sysconfig/proactive-agent

install -d $RPM_BUILD_ROOT/etc/proactive/agent
install -m 644 redhat/agent.xml $RPM_BUILD_ROOT/etc/proactive/agent/agent.xml
install -m 644 redhat/agent.xml $RPM_BUILD_ROOT/etc/proactive/agent/agent.xml

install -d $RPM_BUILD_ROOT%{_localstatedir}/lib/proactive/agent/cgroups
install -d $RPM_BUILD_ROOT%{_localstatedir}/log/proactive/agent/

%clean 
rm -rf $PRM_BUILD_ROOT

%pre
getent group  proactive >/dev/null || groupadd -r proactive || :
getent passwd proactive >/dev/null || useradd  -g proactive \
	-c "ProActive Parallel Suite" -s /sbin/nologin -r -d %{_localstatedir}/lib/proactive proactive 2>/dev/null || :

%post 
chkconfig --add proactive-agent

%postun
/sbin/service proactive-agent condrestart > /dev/null 2>&1 || :

%preun
if [ "$1" = 0 ]
then
        /sbin/service proactive-agent stop > /dev/null 2>&1 || :
        /sbin/chkconfig --del proactive-agent
fi


%files 
%defattr(-, root, root, -)

%dir %{python_sitelib}/palinagent
%{python_sitelib}/palinagent/*
%if %{with_egg}
%{python_sitelib}/ProActive_LinuxAgent-*.egg-info
%{python_sitelib}/ProActive_LinuxAgent-*-nspkg.pth
%endif

%doc README.txt AUTHORS.txt LICENSE.txt INSTALL.txt
%{_mandir}/man1/*
%{_bindir}/proactive-agent

%attr(0755,root,root) /etc/rc.d/init.d/proactive-agent
%attr(0644,root,root) %config(noreplace) %{_sysconfdir}/sysconfig/proactive-agent

%attr(0755,root,root) /etc/proactive/agent
%attr(0644,root,root) %config(noreplace) /etc/proactive/agent/agent.xml

%attr(0700,proactive,proactive)  %{_localstatedir}/lib/proactive/agent/cgroups
%attr(0755,proactive,proactive)  %{_localstatedir}/log/proactive/agent/

%changelog
* Fri Jan 28 2011 Clement MATHIEU <clement.mathieu@activeeon.com> - 1.0.0-0
- Initial RPM

