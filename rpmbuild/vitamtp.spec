# 
# vitamtp spec file
# 

%define _version 2.5.11

%if "%{_version}" == "testing" || "%{_version}" == "master"
%define _verprefix %{_version}
%else
%define _verprefix v%{_version}
%endif

%if 0%{?fedora} || 0%{?centos}
%define _pkgconfig pkgconfig
%define _libusb libusbx-devel
%define _gettext gettext-devel
%define _shadow shadow-utils
%else
%define _pkgconfig pkg-config
%define _libusb libusb-1_0-devel
%define _gettext gettext-tools
%define _shadow shadow
%endif

Name:           libvitamtp
Version:        %{_version}
Release:        0
%define sonum   5
Summary:        Low-level Vita communication library
License:        GPL-3.0-or-later
Group:          System/Libraries
URL:            https://github.com/codestation/vitamtp
Source:         https://github.com/codestation/vitamtp/archive/%{_verprefix}/vitamtp-%{_version}.tar.gz
Requires(pre):  %{_shadow}
BuildRequires:  %{_pkgconfig}
BuildRequires:  libxml2-devel
BuildRequires:  %{_libusb}
BuildRequires:  %{_gettext}
BuildRequires:  libtool

%description
libvitamtp is a library based off of libmtp that does low level USB
communications with the Vita. It can read and receive MTP commands that
the Vita sends, which are a proprietary set of commands that is based on
the MTP open standard.

%package -n %{name}%{sonum}
Summary:        Low-level Vita communication library
Group:          System/Libraries

%description -n %{name}%{sonum}
libvitamtp is a library based off of libmtp that does low level USB
communications with the Vita. It can read and receive MTP commands that
the Vita sends, which are a proprietary set of commands that is based on
the MTP open standard.

%package devel
Summary:        Low-level Vita communication library - development files
Group:          Development/Libraries/C and C++
Requires:       %{name}%{sonum} = %{version}
Requires:       libxml2-devel
Requires:       %{_libusb}

%description devel
libvitamtp is a library based off of libmtp that does low level USB
communications with the Vita. It can read and receive MTP commands that
the Vita sends, which are a proprietary set of commands that is based on
the MTP open standard.
This package contains only the files necessary for development.

%prep
%autosetup -n vitamtp-%{version}

%build
%global _lto_cflags %{?_lto_cflags} -ffat-lto-objects
export CFLAGS="%{optflags}"
export CXXFLAGS="$CFLAGS"
./autogen.sh
%configure
%make_build

%install
%make_install
rm -f %{buildroot}%{_libdir}/*.la
install -Dm644 debian/libvitamtp%{sonum}.udev %{buildroot}%{_udevrulesdir}/60-psvita.rules

%post -n %{name}%{sonum} -p /sbin/ldconfig
%postun -n %{name}%{sonum} -p /sbin/ldconfig

%files -n %{name}%{sonum}
%defattr(-,root,root)
%doc README.md ChangeLog
%license COPYING
%{_libdir}/lib*.so.*
%{_udevrulesdir}/60-psvita.rules

%files devel
%defattr(-,root,root)
%{_includedir}/vitamtp.h
%{_libdir}/libvitamtp.a
%{_libdir}/libvitamtp.so
%{_libdir}/pkgconfig/libvitamtp.pc

%changelog

%pre -n %{name}%{sonum}
getent group vitamtp >/dev/null || groupadd -r vitamtp
echo "NOTE: To use qcma as normal user you have to add yourself into vitamtp group"
exit 0
