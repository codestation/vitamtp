libvitamtp
======================

## What is this?

libvitamtp is a library based off of libMTP that does low level USB
communications with the Vita. It can read and receive MTP commands 
that the Vita sends, which are a proprietary set of commands that is 
based on the MTP open standard.

## What is working?

As of the writing of this, the sending of Vita/PSX/PSP/PSM applications to 
and from the device works. Sending/receiving of PSP saves also work. Backups 
and restoring backups also work. However, edge cases have not been tested, 
and if you run into errors, please report it to the GitHub issues page.

## What needs work?

The major things that are not done yet are 1) media transfers (videos, music, 
and photos), 2) edge cases like "what happens if a file is deleted on the 
computer while it is being transfered?" and 3) testing libvitamtp on multiple 
configurations, hosts, and computers. For the last two, the help of beta 
testers is requested.

## Building

VitaMTP requires CMake 3.21 or newer, libxml2, iconv, and (when USB support
is enabled) libusb 1.0. The default build enables USB and wireless support:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
cmake --install build --prefix /usr/local
```

Use `-DVITAMTP_USB_SUPPORT=OFF` or
`-DVITAMTP_WIRELESS_SUPPORT=OFF` to omit a transport. Documentation can be
built with `-DVITAMTP_BUILD_DOCUMENTATION=ON`. Tests require a C++11 compiler;
use `-DBUILD_TESTING=OFF` for a library-only build.

On Debian/Ubuntu (amd64, arm64, or armhf), install `cmake`, `g++`,
`pkg-config`, `libxml2-dev`, and `libusb-1.0-0-dev`, then use the commands
above. On ARM, use the compiler and dependencies matching the operating
system architecture: 32-bit ARMv7 (armhf) or 64-bit ARM (arm64).
For cross-compilation, pass `-DCMAKE_TOOLCHAIN_FILE=...` pointing to a
CMake toolchain with the target compiler and dependency sysroot.

On Apple Silicon, use native arm64 Homebrew and Xcode command-line tools:

```sh
brew install cmake pkg-config libxml2 libusb
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_PREFIX_PATH="$(brew --prefix libxml2);$(brew --prefix libusb)"
cmake --build build
ctest --test-dir build --output-on-failure
```

On Windows x64, use Visual Studio 2022 with C/C++ tools and vcpkg manifest
mode (the default `x64-windows` triplet):

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Installed consumers can use either CMake or pkg-config. CMake provides
`VitaMTP::VitaMTP` and `VitaMTP::VitaMTPStatic`:

```cmake
find_package(VitaMTP 2.5.13 CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE VitaMTP::VitaMTP)
```

The tests do not require a Vita; USB and Wi-Fi device communication still
requires hardware testing.

## How do I use/test libvitamtp?

Issues Page: https://github.com/codestation/vitamtp/issues

If compilation fails, post the CMake output to the GitHub issues page.

First, note that libVitaMTP is designed with developers in mind. Users should 
stick to [QCMA](https://github.com/codestation/qcma). Full documentation on the interface will be provided in the 
future, but for now, read the interface in vitamtp.h and the function headers 
in vitamtp.c to figure out how functions work and what you can do. OpenCMA has 
been designed with simplicity in mind so there is no harm in reading the code 
and comments for opencma.c and related files. Your implementation should be 
very similar if you wish to replicate CMA functionality.

## Credits?

[Yifan Lu](http://yifan.lu/) has been responsible for this abomination of code

dridri also takes some blame for his help in figuring out many structures and codes
