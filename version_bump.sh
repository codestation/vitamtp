#!/bin/sh
set -eu

[ $# -eq 0 ] && { echo "Usage: $0 <version>"; exit 1; }

version=$1
sed -i "s/%define _version.*/%define _version ${version}/" rpmbuild/vitamtp.spec
sed -i "s/project(VitaMTP VERSION [^ ]*/project(VitaMTP VERSION ${version}/" CMakeLists.txt
sed -i "/\"name\": \"vitamtp\"/,/\"version\":/s/\"version\": \"[^\"]*\"/\"version\": \"${version}\"/" vcpkg.json

echo "Remember to update debian/changelog and release manifests."
