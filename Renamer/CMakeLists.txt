cmake_minimum_required (VERSION 3.12)

set (DIR "Renamer")
set (NAME "Renamer")
set (VERSION "1.0.0")

project (OpenVPN-Config-${DIR} VERSION "${VERSION}" LANGUAGES C CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Build

configure_file ("${CMAKE_BINARY_DIR}/resource.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc" @ONLY)
add_executable ("${NAME}" "${NAME}.cpp" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
set_target_properties ("${NAME}" PROPERTIES VERSION "${VERSION}")
target_link_libraries ("${NAME}" PRIVATE "stdlib")
target_link_libraries ("${NAME}" PRIVATE "inih")

# Test

if (CMAKE_BUILD_TYPE STREQUAL "Test")
	# Program
	add_executable (RenamerTest Test.cpp)
	add_dependencies (RenamerTest Renamer)
	target_link_libraries (RenamerTest PRIVATE gtest gtest_main)
	# Resource
	configure_file ("Help.txt" "${CMAKE_CURRENT_BINARY_DIR}/Help.txt" COPYONLY)
	file (COPY ${CMAKE_SOURCE_DIR}/Templates DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Template.ini" "${CMAKE_CURRENT_BINARY_DIR}/Template.ini" COPYONLY)
	# Register
	add_test (NAME Renamer COMMAND RenamerTest TIMEOUT 5)
endif ()
