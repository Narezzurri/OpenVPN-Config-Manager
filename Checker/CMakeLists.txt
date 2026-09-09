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
	# set (TESTS Checker-Help1 Checker-Help2 Checker-Help3 Checker-Help4 Checker-Function Checker-Wildcard Checker-Bug202609090913)
	# configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Checker-Help.bat.in" "${CMAKE_CURRENT_BINARY_DIR}/Checker-Help.bat" COPYONLY)
	# configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Checker.bat.in" "${CMAKE_CURRENT_BINARY_DIR}/Checker.bat" COPYONLY)
	# configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Help.txt" "${CMAKE_CURRENT_BINARY_DIR}/Help.txt" COPYONLY)
	# add_test (NAME Checker-Help1 COMMAND Checker-Help.bat)
	# add_test (NAME Checker-Help2 COMMAND Checker-Help.bat --help)
	# add_test (NAME Checker-Help3 COMMAND Checker-Help.bat /help)
	# add_test (NAME Checker-Help4 COMMAND Checker-Help.bat /?)
	# add_test (NAME Checker-Function COMMAND Checker.bat ${OVPNS} /w 0.8 /t 2 1 /n 2)
	# add_test (NAME Checker-Wildcard COMMAND Checker.bat ${TEMPLATES}\\*.ovpn /w 0.8 /t 2 1 /n 2)
	# add_test (NAME Checker-Bug202609090913 COMMAND Checker.bat ${TEMPLATES}\\*.ovpn /t 2 /n 2)
	# set_tests_properties (${TESTS} PROPERTIES TIMEOUT 10)
endif ()
