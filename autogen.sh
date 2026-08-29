#!/bin/sh
set -eu

srcdir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
cd "$srcdir"

set -- -fiv -I m4
if [ -d /usr/share/gettext/m4 ]; then
    set -- "$@" -I /usr/share/gettext/m4
fi
exec autoreconf "$@"
