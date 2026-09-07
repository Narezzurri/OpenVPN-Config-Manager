cmake_minimum_required (VERSION 3.12)

set (DIR "Checker")
set (NAME "Checker")
set (VERSION "1.3.0")
# C++ version
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (OpenVPN-Config-${DIR} VERSION "${VERSION}" LANGUAGES C CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Build

configure_file ("${CMAKE_BINARY_DIR}/resource.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc" @ONLY)
add_executable ("${NAME}" "${NAME}.cpp" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
set_target_properties ("${NAME}" PROPERTIES VERSION "${VERSION}")
target_link_libraries ("${NAME}" PRIVATE "stdlib")

# Test

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_test (NAME Checker-Help1 COMMAND Checker --help)
	add_test (NAME Checker-Help2 COMMAND Checker /help)
	add_test (NAME Checker-Help3 COMMAND Checker /?)
	add_test (NAME Checker-Function COMMAND Checker Samples/Sample1.ovpn Samples/Sample2.ovpn Samples/Sample3.ovpn Samples/Sample4.ovpn /w 0.8 /t 2 1 /n 2)
	add_test (NAME Checker-Wildcard COMMAND Checker Samples/*.ovpn /w 0.8 /t 2 1 /n 2)
endif ()
