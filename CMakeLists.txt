cmake_minimum_required (VERSION 3.12)

set (VERSION "2.2.0")
# Target list
set (APPLIST "OCM.Checker;OCM.Auth")
set (SETUP_DEPEND ${CMAKE_BINARY_DIR}/Setup.nsi Checker Auth)
# C++ version
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (OpenVPN-Config-Manager VERSION "${VERSION}" LANGUAGES CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Test

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	enable_testing ()
	set (CTEST_TIMEOUT 10)
	add_compile_definitions (DEBUG)
endif ()

# Dependances

include_directories (.)
add_library ("lib" STATIC "lib.cpp")
set (AT "@")
configure_file ("resource.rc.in.in" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc.in" @ONLY)

# Targets

add_subdirectory ("Checker")
add_subdirectory ("Authenticator")

# Install package

configure_file ("Setup.nsi.in" "Setup.nsi" @ONLY)
add_custom_target (
	build
	COMMENT "Creating NSIS installer"
	DEPENDS ${SETUP_DEPEND}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E echo "Start Building NSIS installer..."
	COMMAND makensis "${CMAKE_BINARY_DIR}/Setup.nsi"
	COMMAND ${CMAKE_COMMAND} -E echo "Done."
)
