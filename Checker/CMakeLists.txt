cmake_minimum_required (VERSION 3.12)

set (DIR "Checker")
set (NAME "Checker")
set (VERSION "1.3.0")

project (OpenVPN-Config-${DIR} VERSION "${VERSION}" LANGUAGES C CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Build

configure_file ("${CMAKE_BINARY_DIR}/resource.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc" @ONLY)
add_executable ("${NAME}" "${NAME}.cpp" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
set_target_properties ("${NAME}" PROPERTIES VERSION "${VERSION}")
target_link_libraries ("${NAME}" PRIVATE "stdlib")

# Test

if (CMAKE_BUILD_TYPE STREQUAL "Test")
# Program
	add_executable (CheckerTest Test.cpp)
	add_dependencies (CheckerTest Checker)
	target_link_libraries (CheckerTest PRIVATE gtest gtest_main)
# Resource
	configure_file ("Help.txt" "${CMAKE_CURRENT_BINARY_DIR}/Help.txt" COPYONLY)
	file (COPY ${CMAKE_SOURCE_DIR}/Templates DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# Register
	add_test (NAME Checker COMMAND CheckerTest)
endif ()
