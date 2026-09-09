include(FetchContent)
cmake_minimum_required (VERSION 3.12)

set (DIR "Renamer")
set (NAME "Renamer")
set (VERSION "1.0.0")
# C++ version
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (OpenVPN-Config-${DIR} VERSION "${VERSION}" LANGUAGES C CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Build

configure_file ("${CMAKE_BINARY_DIR}/resource.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc" @ONLY)
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Template.ini" "${CMAKE_CURRENT_BINARY_DIR}/Template.ini" COPYONLY)
add_executable ("${NAME}" "${NAME}.cpp" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
set_target_properties ("${NAME}" PROPERTIES VERSION "${VERSION}")
target_link_libraries ("${NAME}" PRIVATE "stdlib")
target_link_libraries ("${NAME}" PRIVATE "inih")

# Test

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# set (TESTS Renamer-Help1 Renamer-Help2 Renamer-Help3 Renamer-Help4 Renamer-Function Renamer-Wildcard)
	# configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Renamer-Help.bat.in" "${CMAKE_CURRENT_BINARY_DIR}/Renamer-Help.bat" COPYONLY)
	# configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Renamer.bat.in" "${CMAKE_CURRENT_BINARY_DIR}/Renamer.bat" COPYONLY)
	# configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Help.txt" "${CMAKE_CURRENT_BINARY_DIR}/Help.txt" COPYONLY)
	# add_test (NAME Renamer-Help1 COMMAND Renamer-Help.bat)
	# add_test (NAME Renamer-Help2 COMMAND Renamer-Help.bat --help)
	# add_test (NAME Renamer-Help3 COMMAND Renamer-Help.bat /help)
	# add_test (NAME Renamer-Help4 COMMAND Renamer-Help.bat /?)
	# add_test (NAME Renamer-Function COMMAND Renamer.bat ${CMAKE_CURRENT_SOURCE_DIR}\\Template.ini ${OVPNS})
	# add_test (NAME Renamer-Wildcard COMMAND Renamer.bat ${CMAKE_CURRENT_SOURCE_DIR}\\Template.ini ${TEMPLATES}\\*.ovpn)
	# set_tests_properties (${TESTS} PROPERTIES TIMEOUT 10)
endif ()
