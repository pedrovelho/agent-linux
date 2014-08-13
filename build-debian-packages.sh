#!/bin/bash

set -e

VERSION=${VERSION:-1.0.3}
JRE_VERSION=${JRE_VERSION:-7u67-b01}
[ -n "$NODE" ] || ( echo "NODE variable should point to the unpacked ProActiveNode"; exit 1 )

THIS=$(cd $(dirname $0); pwd)
BUILD=$THIS/build/

WGET="wget -N --no-check-certificate --no-cookies"

download_jres () {
    mkdir -p $BUILD/jre
    $WGET --header "Cookie: oraclelicense=accept-securebackup-cookie" -P $BUILD/jre http://download.oracle.com/otn-pub/java/jdk/$JRE_VERSION/jre-${JRE_VERSION%-*}-linux-x64.tar.gz
    $WGET --header "Cookie: oraclelicense=accept-securebackup-cookie" -P $BUILD/jre http://download.oracle.com/otn-pub/java/jdk/$JRE_VERSION/jre-${JRE_VERSION%-*}-linux-i586.tar.gz
}

package () {
    local ARCH=$1
    local PREFIX=$BUILD/$ARCH

    mkdir -p $PREFIX
    rsync -avP --delete $THIS/packaging/debian/ $PREFIX/
    sed -i -e "s/VERSION/$VERSION/" -e "s/ARCH/$ARCH/" $PREFIX/DEBIAN/control

    mkdir -p $PREFIX/opt/proactive-agent
    rsync -avP --delete $THIS/proactive-agent $THIS/proactive-agent.1 $THIS/palinagent $THIS/config* $THIS/data $THIS/LICENSE.txt \
        $PREFIX/opt/proactive-agent/

    mkdir -p $PREFIX/opt/proactive-node
    rsync -avP --delete $NODE/ $PREFIX/opt/proactive-node/

    mkdir -p $PREFIX/opt/jre
    case $ARCH in
        amd64)
            local JRE=$BUILD/jre/jre-${JRE_VERSION%-*}-linux-x64.tar.gz
            ;;
        i386)
            local JRE=$BUILD/jre/jre-${JRE_VERSION%-*}-linux-i586.tar.gz
    esac
    tar -C $PREFIX/opt/jre --strip-components 1 -xf $JRE

    mkdir -p $BUILD/distributions
    fakeroot dpkg -b $PREFIX/ $BUILD/distributions/proactive-agent_${VERSION}_$ARCH.deb
}

download_jres

for arch in amd64 i386; do
    package $arch
done
