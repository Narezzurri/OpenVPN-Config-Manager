cmake_minimum_required (VERSION 3.12)

# Component list
set (EXES CHECKER AUTH RENAMER)
set (APPLIST "OCM.Checker;OCM.Auth;OCM.Renamer")
set (SETUP_DEPEND ${CMAKE_BINARY_DIR}/Setup.nsi Checker Auth Renamer)
# Target name
set (CHECKER_NAME "Checker")
set (AUTH_NAME "Auth")
set (RENAMER_NAME "Renamer")
# Full name
set (CHECKER_DIR "Checker")
set (AUTH_DIR "Authenticator")
set (RENAMER_DIR "Renamer")
# Version
set (VERSION "2.2.0")
set (CHECKER_VER "1.3.0")
set (AUTH_VER "1.0.0")
set (RENAMER_VER "1.0.0")
# C++ version
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Functions
function (split VERSION MAJOR MINOR PATCH)
	string (REPLACE "." ";" VERSION "${VERSION}")
	list (GET VERSION 0 major)
	list (GET VERSION 1 minor)
	list (GET VERSION 2 patch)
	set (${MAJOR} ${major} PARENT_SCOPE)
	set (${MINOR} ${minor} PARENT_SCOPE)
	set (${PATCH} ${patch} PARENT_SCOPE)
endfunction ()

project (OpenVPN-Config-Manager VERSION "${VERSION}" LANGUAGES CXX HOMEPAGE_URL https://github.com/Narezzurri/OpenVPN-Config-Manager)

# Construction

include_directories (.)
add_library (lib STATIC lib.cpp)

foreach (EXE ${EXES})
	set (OBJ ${${EXE}_NAME})
	add_executable ("${OBJ}" "${CMAKE_CURRENT_SOURCE_DIR}/${${EXE}_DIR}/${OBJ}.cpp")
	set_target_properties ("${OBJ}" PROPERTIES VERSION "${${EXE}_VER}" OUTPUT_NAME "${OBJ}")
	# resource
	set (APP_NAME "${OBJ}")
	set (APP_FULLNAME "${${EXE}_DIR}")
	split ("${${EXE}_VER}" APP_VERSION_MAJOR APP_VERSION_MINOR APP_VERSION_PATCH)
	configure_file (${CMAKE_CURRENT_SOURCE_DIR}/resource.rc.in "${CMAKE_CURRENT_BINARY_DIR}/${OBJ}/resource.rc" @ONLY)
	target_sources ("${OBJ}" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${OBJ}/resource.rc")
	# library
	target_link_libraries ("${OBJ}" lib)
endforeach ()
target_sources ("Renamer" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Renamer/ini.c")
target_sources ("Renamer" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Renamer/cpp/INIReader.cpp")

# Test

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	enable_testing ()
	set (CTEST_TIMEOUT 10)
	add_compile_definitions (DEBUG)
	# Checker
	add_test (NAME Checker_Help1 COMMAND Checker --help)
	add_test (NAME Checker_Help2 COMMAND Checker /help)
	add_test (NAME Checker_Help3 COMMAND Checker /?)
	add_test (NAME Checker_Function COMMAND Checker Templates/Template1.ovpn Templates/Template2.ovpn Templates/Template3.ovpn Templates/Template4.ovpn /w 0.8 /t 2 1 /n 2)
	add_test (NAME Checker_Wildcard COMMAND Checker Templates/*.ovpn /w 0.8 /t 2 1 /n 2)
	# Authenticator
	add_test (NAME Auth_Help1 COMMAND Auth --help)
	add_test (NAME Auth_Help2 COMMAND Auth /help)
	add_test (NAME Auth_Help3 COMMAND Auth /?)
	add_test (NAME Auth_Function COMMAND Auth Templates/Template1.ovpn Templates/Template2.ovpn Templates/Template3.ovpn Templates/Template4.ovpn)
	add_test (NAME Auth_Wildcard COMMAND Auth Templates/*.ovpn)
endif ()

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

# install (
# 	TARGETS CHECKER AUTH
# 	RUNTIME DESTINATION "."
# 	ARCHIVE DESTINATION lib
# )
# install (FILES "LICENSE" DESTINATION ".")
# install (FILES "NOTICE" DESTINATION ".")

# set (CPACK_GENERATOR "NSIS")
# set (CPACK_NSIS_LANGUAGE "English")
# set (CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
# set (CPACK_PACKAGE_NAME "OpenVPN Config Manager")
# set (CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_NAME}")
# set (CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
# set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
# set (CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/resource/InstallerLight.ico")
# set (CPACK_NSIS_MUI_UNICON "${CMAKE_SOURCE_DIR}/resource/UninstallerLight.ico")
# set (CPACK_PACKAGE_FILE_NAME "OpenVPNConfigManagerSetup-${CPACK_PACKAGE_VERSION}")

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

# include (CPack)
