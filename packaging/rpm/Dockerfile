FROM centos

RUN yum -y install @development-tools wget ruby ruby-devel

RUN /usr/sbin/useradd --create-home --shell /bin/bash --uid 1000 packager

USER packager

ENV HOME /home/packager
ENV GEM_HOME /home/packager/gems

RUN gem install fpm

