cmake_minimum_required (VERSION 3.12)

set (VERSION "1.0.0")
# C++ version
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (OpenVPN-Config-Library VERSION "${VERSION}" LANGUAGES C CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

add_library (lib SHARED "lib.cpp" "ini.c" "cpp/INIReader.cpp")
target_include_directories (lib PUBLIC "." "cpp")
set_target_properties (lib PROPERTIES VERSION "${VERSION}" OUTPUT_NAME "rary")
