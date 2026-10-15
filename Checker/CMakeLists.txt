cmake_minimum_required (VERSION 3.12)

set (DIR "Checker")
set (NAME "Checker")
set (VERSION "1.4.0")

project (OpenVPN-Config-${DIR} VERSION "${VERSION}" LANGUAGES C CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Build

configure_file ("${CMAKE_BINARY_DIR}/resource.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc" @ONLY)
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Help.txt.in" "${CMAKE_CURRENT_BINARY_DIR}/Help.txt" @ONLY)
add_executable ("${NAME}" "${NAME}.cpp" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options ("${NAME}" PRIVATE "--embed-dir=${CMAKE_CURRENT_BINARY_DIR}")
else ()
    target_include_directories ("${NAME}" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
endif ()
set_target_properties ("${NAME}" PROPERTIES VERSION "${VERSION}")
target_link_libraries ("${NAME}" PRIVATE stdlib)

# Test

if (CMAKE_BUILD_TYPE STREQUAL "Test")
	# Program
	add_executable (CheckerTest Test.cpp)
	add_dependencies (CheckerTest Checker)
	target_link_libraries (CheckerTest PRIVATE gtest gtest_main stdlib)
	# Register
	add_test (NAME Checker COMMAND CheckerTest)
	set_tests_properties (Checker PROPERTIES TIMEOUT 25)
endif ()
