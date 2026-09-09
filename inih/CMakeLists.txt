cmake_minimum_required (VERSION 3.12)

set (VERSION "62")
# C++ version
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (inih VERSION "${VERSION}" LANGUAGES C CXX HOMEPAGE_URL https://github.com/benhoyt/inih)

add_library (inih SHARED "ini.c" "cpp/INIReader.cpp")
target_include_directories (inih PUBLIC "." "cpp")
set_target_properties (inih PROPERTIES VERSION "${VERSION}" SOVERSION "${PROJECT_VERSION_MAJOR}" PREFIX "" OUTPUT_NAME "inih")
