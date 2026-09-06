enable_language(CXX)
# Keep test executables alongside the DLL for Windows runtime lookup.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_compile_definitions(
  TEST_USB_SUPPORT=$<BOOL:${VITAMTP_USB_SUPPORT}>
  TEST_WIRELESS_SUPPORT=$<BOOL:${VITAMTP_WIRELESS_SUPPORT}>)

add_executable(vitamtp_shared_test api_test.c)
target_link_libraries(vitamtp_shared_test PRIVATE VitaMTP::VitaMTP)
add_test(NAME vitamtp.shared COMMAND vitamtp_shared_test)

add_executable(vitamtp_static_test api_test.c)
target_compile_definitions(vitamtp_static_test PRIVATE VITAMTP_STATIC)
target_link_libraries(vitamtp_static_test PRIVATE VitaMTP::VitaMTPStatic)
add_test(NAME vitamtp.static COMMAND vitamtp_static_test)

add_executable(vitamtp_cpp_header_test header_test.cpp)
target_compile_features(vitamtp_cpp_header_test PRIVATE cxx_std_11)
target_link_libraries(vitamtp_cpp_header_test PRIVATE VitaMTP::VitaMTP)
add_test(NAME vitamtp.cpp-header COMMAND vitamtp_cpp_header_test)

add_executable(vitamtp_endian_test endian_test.c)
target_include_directories(vitamtp_endian_test PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_compile_features(vitamtp_endian_test PRIVATE c_std_99)
if(WIN32)
  target_link_libraries(vitamtp_endian_test PRIVATE ws2_32)
endif()
add_test(NAME vitamtp.endian COMMAND vitamtp_endian_test)

add_executable(vitamtp_asprintf_test asprintf_test.c ../src/asprintf.c)
target_include_directories(vitamtp_asprintf_test PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_compile_features(vitamtp_asprintf_test PRIVATE c_std_99)
add_test(NAME vitamtp.asprintf COMMAND vitamtp_asprintf_test)

add_executable(vitamtp_datautils_test datautils_test.c)
target_link_libraries(vitamtp_datautils_test PRIVATE VitaMTP::VitaMTP)
add_test(NAME vitamtp.datautils COMMAND vitamtp_datautils_test)
set_tests_properties(vitamtp.datautils PROPERTIES ENVIRONMENT "TZ=UTC0")

add_executable(vitamtp_ptp_pack_test ptp_pack_test.c)
target_include_directories(vitamtp_ptp_pack_test PRIVATE "${PROJECT_BINARY_DIR}/include")
target_link_libraries(vitamtp_ptp_pack_test PRIVATE VitaMTP::VitaMTPStatic)
add_test(NAME vitamtp.ptp-pack COMMAND vitamtp_ptp_pack_test)

foreach(linkage shared static)
  add_executable(vitamtp_logging_${linkage}_test logging_test.c)
  if(linkage STREQUAL "shared")
    target_link_libraries(vitamtp_logging_${linkage}_test PRIVATE VitaMTP::VitaMTP)
  else()
    target_link_libraries(vitamtp_logging_${linkage}_test PRIVATE VitaMTP::VitaMTPStatic)
  endif()
  add_test(NAME vitamtp.logging-${linkage}
    COMMAND "${CMAKE_COMMAND}"
      "-DTEST_EXECUTABLE=$<TARGET_FILE:vitamtp_logging_${linkage}_test>"
      -P "${CMAKE_CURRENT_SOURCE_DIR}/check_logging.cmake")
endforeach()
