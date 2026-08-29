#!/bin/sh
set -eu

repo_dir=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$repo_dir"

version=${1:-}
obs_package=${2:-}
if [ -z "$version" ] || [ -z "$obs_package" ]; then
    echo "Usage: $0 <version> <OBS vitamtp package directory>" >&2
    exit 2
fi

expected=$(sed -n 's/^AC_INIT(\[vitamtp\], \[\([^]]*\)\].*/\1/p' configure.ac)
if [ "$version" != "$expected" ]; then
    echo "Version mismatch: configure.ac contains $expected" >&2
    exit 1
fi
if ! git diff --quiet || ! git diff --cached --quiet; then
    echo "Refusing to create release artifacts from a dirty tree" >&2
    exit 1
fi

epoch=$(git log -1 --format=%ct)
tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT HUP INT TERM
archive="vitamtp-$version.tar.gz"
git archive --format=tar --prefix="vitamtp-$version/" HEAD | gzip -n -9 >"$tmpdir/$archive"
mkdir "$tmpdir/debian"
cp -a debian/. "$tmpdir/debian/"
tar --sort=name --mtime="@$epoch" --owner=0 --group=0 --numeric-owner -C "$tmpdir" -cJf "$tmpdir/debian.tar.xz" debian

size=$(wc -c <"$tmpdir/$archive" | tr -d ' ')
sha1=$(sha1sum "$tmpdir/$archive" | cut -d' ' -f1)
sha256=$(sha256sum "$tmpdir/$archive" | cut -d' ' -f1)
md5=$(md5sum "$tmpdir/$archive" | cut -d' ' -f1)
sed "s/@VERSION@/$version/g" buildscripts/debian/vitamtp.dsc.in >"$tmpdir/vitamtp.dsc"
cat >>"$tmpdir/vitamtp.dsc" <<EOF
Checksums-Sha1:
 $sha1 $size $archive
Checksums-Sha256:
 $sha256 $size $archive
Files:
 $md5 $size $archive
EOF

install -m644 "$tmpdir/$archive" "$obs_package/$archive"
install -m644 "$tmpdir/debian.tar.xz" "$obs_package/debian.tar.xz"
install -m644 "$tmpdir/vitamtp.dsc" "$obs_package/vitamtp.dsc"
install -m644 rpmbuild/vitamtp.spec "$obs_package/vitamtp.spec"
install -m644 rpmbuild/vitamtp.changes "$obs_package/vitamtp.changes"
echo "Prepared vitamtp $version in $obs_package"
