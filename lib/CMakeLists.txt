cmake_minimum_required (VERSION 3.12)

set (NAME "lib")
set (VERSION "1.0.0")
# C++ version
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (OpenVPN-Config-${NAME} VERSION "${VERSION}" LANGUAGES CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Build

add_library ("${NAME}" STATIC "lib.cpp" "ini.c" "cpp/INIReader.cpp")
target_include_directories ("${NAME}" PUBLIC cpp .)
set_target_properties ("${NAME}" PROPERTIES VERSION "${VERSION}" SOVERSION "${PROJECT_VERSION_MAJOR}")
