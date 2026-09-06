cmake_minimum_required (VERSION 3.12)

set (DIR "Renamer")
set (NAME "Renamer")
set (VERSION "1.0.0")
# C++ version
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (OpenVPN-Config-Renamer VERSION "${VERSION}" LANGUAGES CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Build

include_directories ("." ".." "cpp")

configure_file ("../resource.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/${DIR}/resource.rc" @ONLY)
add_executable ("DIR" "DIR.cpp" "${CMAKE_CURRENT_BINARY_DIR}/${DIR}/resource.rc" "ini.c" "cpp/INIReader.cpp")
set_target_properties ("DIR" PROPERTIES VERSION "${VERSION}")

# Test

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	enable_testing ()
	set (CTEST_TIMEOUT 10)
	add_compile_definitions (DEBUG)
endif ()
