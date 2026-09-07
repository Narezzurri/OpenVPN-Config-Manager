cmake_minimum_required (VERSION 3.12)

# Target list
set (SUBDIR Checker Authenticator Renamer)
set (APPLIST "OCM.Checker;OCM.Auth;OCM.Renamer")
set (SETUP_DEPEND ${CMAKE_BINARY_DIR}/Setup.nsi Checker Auth Renamer)
# Version
set (VERSION "2.2.0")
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (OpenVPN-Config-Manager VERSION "${VERSION}" LANGUAGES C CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Debug

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	enable_testing ()
	set (CTEST_TIMEOUT 10)
	add_compile_definitions (DEBUG)
endif ()

# Dependances

set (AT "@")
configure_file ("resource.rc.in.in" "${CMAKE_CURRENT_BINARY_DIR}/resource.rc.in" @ONLY)

# Targets

include_directories (".")

add_subdirectory ("inih")
add_subdirectory ("stdlib")

foreach (DIR ${SUBDIR})
	add_subdirectory ("${DIR}")
endforeach ()

# Install package

configure_file ("Setup.nsi.in" "Setup.nsi" @ONLY)
add_custom_command (
	OUTPUT OpenVPNConfigManagerSetup-${VERSION}.exe
	DEPENDS ${SETUP_DEPEND}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Generating NSIS installer"
	COMMAND makensis "${CMAKE_BINARY_DIR}/Setup.nsi"
)

add_custom_target (
	build
	DEPENDS ${SETUP_DEPEND}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Creating NSIS installer"
)

add_custom_target (
	deploy
	DEPENDS OpenVPNConfigManagerSetup-${VERSION}.exe
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Deploying ${PROJECT_NAME}"
	COMMAND ./OpenVPNConfigManagerSetup-${VERSION}.exe /S
)
