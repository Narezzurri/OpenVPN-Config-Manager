cmake_minimum_required (VERSION 3.12)

set (DIR "Renamer")
set (NAME "Renamer")
set (VERSION "1.0.1")

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
target_link_libraries ("${NAME}" PRIVATE stdlib inih)

# Test

if (CMAKE_BUILD_TYPE STREQUAL "Test")
	# Program
	add_executable (RenamerTest Test.cpp)
	add_dependencies (RenamerTest Renamer)
	target_link_libraries (RenamerTest PRIVATE gtest gtest_main stdlib)
	# Resource
	configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Template.ini" "${CMAKE_BINARY_DIR}/Templates/Template.ini" COPYONLY)
	# Register
	add_test (NAME Renamer COMMAND RenamerTest)
	set_tests_properties (Renamer PROPERTIES TIMEOUT 8)
endif ()
