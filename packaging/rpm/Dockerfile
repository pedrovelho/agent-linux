FROM centos

RUN yum -y install wget ruby ruby-devel gcc make

RUN /usr/sbin/useradd --create-home --shell /bin/bash --uid 1000 packager

USER packager

ENV HOME /home/packager
ENV GEM_HOME /home/packager/gems

RUN gem install fpm
