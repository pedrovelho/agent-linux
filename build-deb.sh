#!/bin/bash

VERSION=1.0.3

mkdir -p packaging/debian/opt/proactive-agent

rsync -avP --delete proactive-agent proactive-agent.1 palinagent config* data LICENSE.txt \
    packaging/debian/opt/proactive-agent/

dpkg -b packaging/debian/ proactive-agent_${VERSION}_all.deb
