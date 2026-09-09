cmake_minimum_required (VERSION 3.12)

set (VERSION "1.0.0")
# C++ version
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (OpenVPN-Config-Library VERSION "${VERSION}" LANGUAGES C CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

add_library (stdlib SHARED "lib.cpp")
target_include_directories (stdlib PUBLIC ".")
set_target_properties (stdlib PROPERTIES VERSION "${VERSION}" SOVERSION "${PROJECT_VERSION_MAJOR}" PREFIX "" OUTPUT_NAME "stdlib")
target_link_libraries (stdlib PUBLIC "inih")
