#!/bin/sh

[ $# -eq 0 ] && { echo "Usage: $0 <version>"; exit 1; }

sed -i "s/%define _version.*/%define _version $1/" rpmbuild/vitamtp.spec
sed -i "s/AC_INIT(\[vitamtp\], \[.*\], \[codestation@gmail.com\]/AC_INIT(\[vitamtp\], \[$1\], \[codestation@gmail.com\]/" configure.ac

echo "Don't forget to update the changelog"
