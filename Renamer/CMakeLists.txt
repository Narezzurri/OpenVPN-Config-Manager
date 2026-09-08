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
	add_test (NAME Renamer-Help1 COMMAND Renamer --help)
	add_test (NAME Renamer-Help2 COMMAND Renamer /help)
	add_test (NAME Renamer-Help3 COMMAND Renamer /?)
	add_test (NAME Renamer-Function COMMAND Renamer ${CMAKE_CURRENT_SOURCE_DIR}\\Template.ini ${TEMPLATES}\\ca-3.tcp.ovpn ${TEMPLATES}\\jp-4.tcp.ovpn ${TEMPLATES}\\uk-2.udp.ovpn ${TEMPLATES}\\us-1.udp.ovpn)
	add_test (NAME Renamer-Wildcard COMMAND Renamer ${CMAKE_CURRENT_SOURCE_DIR}\\Template.ini ${TEMPLATES}\\*.ovpn)
endif ()
