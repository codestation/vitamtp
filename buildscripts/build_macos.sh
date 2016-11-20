#!/bin/bash

set -eu

show_usage() {
    echo -e "Usage: $0 <host> <branch>"
}

if [ $# -lt 1 ]
then
    show_usage
    exit 1
fi

SERVER_HOST=$1
BRANCH=$2

VITAMTP_SOURCES=~/projects/vitamtp

VERSION=$(git -C "${VITAMTP_SOURCES}" describe --tags --abbrev=8)
VERSION=${VERSION#v*}

git -C "${VITAMTP_SOURCES}" bundle create vitamtp.bundle --all
scp vitamtp.bundle ${SERVER_HOST}:vitamtp.bundle

ssh -T "${SERVER_HOST}" << EOSSH
#!/bin/bash

set -eu
VITAMTP_DIR="\${HOME}/vitamtp"
PATH=/usr/local/bin:\$PATH

rm -rf vitamtp
git clone vitamtp.bundle vitamtp

pushd vitamtp
sed -i "" -e "s/libtoolize/glibtoolize/" \${VITAMTP_DIR}/autogen.sh
\${VITAMTP_DIR}/autogen.sh
popd

rm -rf vitamtp_build
mkdir vitamtp_build
pushd vitamtp_build

# required by brew installation
export LDFLAGS=-L/usr/local/opt/libxml2/lib
export CPPFLAGS=-I/usr/local/opt/libxml2/include
export PKG_CONFIG_PATH=/usr/local/opt/libxml2/lib/pkgconfig

\${VITAMTP_DIR}/configure
make -j2
make install
popd
EOSSH
