#!/bin/sh
set -eu

[ $# -eq 0 ] && { echo "Usage: $0 <version>"; exit 1; }

version=$1
sed -i "s/%define _version.*/%define _version ${version}/" rpmbuild/vitamtp.spec
sed -i "s/AC_INIT(\[vitamtp\], \[[^]]*\]/AC_INIT([vitamtp], [${version}]/" configure.ac

echo "Remember to update debian/changelog and release manifests."
