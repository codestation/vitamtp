cmake_minimum_required(VERSION 3.21)
project(VitaMTP VERSION 2.5.11 LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CTest)

option(VITAMTP_USB_SUPPORT "Build USB transport support" ON)
option(VITAMTP_WIRELESS_SUPPORT "Build wireless transport support" ON)
option(VITAMTP_BUILD_DOCUMENTATION "Build API documentation with Doxygen" OFF)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(LibXml2 REQUIRED)
find_package(Iconv REQUIRED)
if(NOT WIN32)
  find_package(Threads REQUIRED)
endif()
if(VITAMTP_USB_SUPPORT)
  find_package(LibUSB REQUIRED)
  set(VITAMTP_PC_LIBUSB " libusb-1.0")
else()
  set(VITAMTP_PC_LIBUSB "")
endif()

set(VITAMTP_SOURCES src/datautils.c src/device.c src/ptp.c src/usb.c src/vitamtp.c src/wireless.c)
if(WIN32)
  list(APPEND VITAMTP_SOURCES src/asprintf.c
    $<$<BOOL:${VITAMTP_WIRELESS_SUPPORT}>:src/socketpair.c>)
endif()

function(vitamtp_configure_target target)
  target_compile_features(${target} PRIVATE c_std_99)
  set_target_properties(${target} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  target_include_directories(${target} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
  target_link_libraries(${target} PUBLIC LibXml2::LibXml2 Iconv::Iconv)
  if(WIN32)
    target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_link_libraries(${target} PUBLIC ws2_32)
  else()
    target_link_libraries(${target} PUBLIC Threads::Threads)
  endif()
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wno-unused-parameter -Wno-parentheses)
  endif()
  if(VITAMTP_USB_SUPPORT)
    target_compile_definitions(${target} PRIVATE PTP_USB_SUPPORT)
    target_link_libraries(${target} PUBLIC LibUSB::LibUSB)
  endif()
  if(VITAMTP_WIRELESS_SUPPORT)
    target_compile_definitions(${target} PRIVATE PTP_IP_SUPPORT)
  endif()
endfunction()

add_library(VitaMTP SHARED ${VITAMTP_SOURCES})
add_library(VitaMTP::VitaMTP ALIAS VitaMTP)
vitamtp_configure_target(VitaMTP)
target_compile_definitions(VitaMTP PRIVATE VITAMTP_BUILDING_DLL)
set_target_properties(VitaMTP PROPERTIES OUTPUT_NAME vitamtp VERSION 5.0.1 SOVERSION 5)
if(UNIX AND NOT APPLE)
  target_link_options(VitaMTP PRIVATE "LINKER:--version-script=${CMAKE_CURRENT_SOURCE_DIR}/cmake/vitamtp.map")
  set_property(TARGET VitaMTP APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/vitamtp.map")
elseif(APPLE)
  # Preserve libtool -version-info 5:1:0's Mach-O load-command versions.
  set_target_properties(VitaMTP PROPERTIES
    MACHO_COMPATIBILITY_VERSION 6.0.0 MACHO_CURRENT_VERSION 6.1.0)
  target_link_options(VitaMTP PRIVATE "LINKER:-exported_symbol,_VitaMTP_*")
endif()

add_library(VitaMTPStatic STATIC ${VITAMTP_SOURCES})
add_library(VitaMTP::VitaMTPStatic ALIAS VitaMTPStatic)
vitamtp_configure_target(VitaMTPStatic)
target_compile_definitions(VitaMTPStatic PUBLIC VITAMTP_STATIC)
if(WIN32)
  set_target_properties(VitaMTPStatic PROPERTIES OUTPUT_NAME vitamtp-static)
else()
  set_target_properties(VitaMTPStatic PROPERTIES OUTPUT_NAME vitamtp)
endif()

configure_file(cmake/config.h.in include/config.h @ONLY)
set(VITAMTP_PC_PRIVATE_LIBS "${CMAKE_THREAD_LIBS_INIT}")
if(NOT Iconv_IS_BUILT_IN)
  string(APPEND VITAMTP_PC_PRIVATE_LIBS " -liconv")
endif()
if(WIN32)
  string(APPEND VITAMTP_PC_PRIVATE_LIBS " -lws2_32")
endif()
foreach(dir LIBDIR INCLUDEDIR)
  if(IS_ABSOLUTE "${CMAKE_INSTALL_${dir}}")
    set(VITAMTP_PC_${dir} "${CMAKE_INSTALL_${dir}}")
  else()
    set(VITAMTP_PC_${dir} "\${prefix}/${CMAKE_INSTALL_${dir}}")
  endif()
endforeach()
configure_file(libvitamtp.pc.in libvitamtp.pc @ONLY)

install(TARGETS VitaMTP VitaMTPStatic EXPORT VitaMTPTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES src/vitamtp.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libvitamtp.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

set(VITAMTP_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/VitaMTP")
configure_package_config_file(cmake/VitaMTPConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/VitaMTPConfig.cmake"
  INSTALL_DESTINATION "${VITAMTP_CMAKE_INSTALL_DIR}")
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/VitaMTPConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}" COMPATIBILITY SameMajorVersion)
install(EXPORT VitaMTPTargets NAMESPACE VitaMTP:: DESTINATION "${VITAMTP_CMAKE_INSTALL_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VitaMTPConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/VitaMTPConfigVersion.cmake" cmake/FindLibUSB.cmake
  DESTINATION "${VITAMTP_CMAKE_INSTALL_DIR}")

if(VITAMTP_BUILD_DOCUMENTATION)
  find_package(Doxygen REQUIRED)
  set(VERSION "${PROJECT_VERSION}")
  set(top_srcdir "${CMAKE_CURRENT_SOURCE_DIR}")
  configure_file(doc/Doxyfile.in doc/Doxyfile @ONLY)
  add_custom_target(doc ALL COMMAND Doxygen::doxygen "${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile")
endif()
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
