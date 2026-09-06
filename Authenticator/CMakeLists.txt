cmake_minimum_required (VERSION 3.12)

set (NAME "Auth")
set (VERSION "1.0.0")
set (DIR "Authenticator")
# C++ version
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (OpenVPN-Config-${DIR} VERSION "${VERSION}" LANGUAGES CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Build

configure_file ("${CMAKE_BINARY_DIR}/resource.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc" @ONLY)
add_executable ("${NAME}" "${NAME}.cpp" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
set_target_properties ("${NAME}" PROPERTIES VERSION "${VERSION}")
target_link_libraries ("${NAME}" PRIVATE "lib")

# Test

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_test (NAME Auth_Help1 COMMAND Auth --help)
	add_test (NAME Auth_Help2 COMMAND Auth /help)
	add_test (NAME Auth_Help3 COMMAND Auth /?)
	add_test (NAME Auth_Function COMMAND Auth Samples/Sample1.ovpn Samples/Sample2.ovpn Samples/Sample3.ovpn Samples/Sample4.ovpn)
	add_test (NAME Auth_Wildcard COMMAND Auth Samples/*.ovpn)
endif ()
