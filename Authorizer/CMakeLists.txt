cmake_minimum_required (VERSION 3.12)

set (NAME "Auth")
set (VERSION "1.0.0")
set (DIR "Authorizer")
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

set (INIT "copy ${TEMPLATES}\\* Templates /Y")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# set (TESTS Auth-Help1 Auth-Help2 Auth-Help3 Auth-Help4 Auth-Function Auth-Wildcard)
	# configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Auth.bat.in" "${CMAKE_CURRENT_BINARY_DIR}/Auth.bat" COPYONLY)
	# configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Auth-Help.bat.in" "${CMAKE_CURRENT_BINARY_DIR}/Auth-Help.bat" COPYONLY)
	# configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Auth-Skip.bat.in" "${CMAKE_CURRENT_BINARY_DIR}/Auth-Skip.bat" COPYONLY)
	# add_test (NAME Auth-Help1 COMMAND Auth-Help.bat)
	# add_test (NAME Auth-Help2 COMMAND Auth-Help.bat --help)
	# add_test (NAME Auth-Help3 COMMAND Auth-Help.bat /help)
	# add_test (NAME Auth-Help4 COMMAND Auth-Help.bat /?)
	# add_test (NAME Auth-Function COMMAND Auth.bat ${OVPNS})
	# add_test (NAME Auth-Wildcard COMMAND Auth.bat ${TEMPLATES}\\*.ovpn)
	# add_test (NAME Auth-Skip COMMAND Auth-Skip.bat ${TEMPLATES}\\*.ovpn)
	# set_tests_properties (${TESTS} PROPERTIES TIMEOUT 10)
endif ()
