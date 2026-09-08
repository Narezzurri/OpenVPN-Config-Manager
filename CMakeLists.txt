cmake_minimum_required (VERSION 3.12)

# Target list
set (SUBDIR Checker Authenticator Renamer)
set (APPLIST "OCM.Checker;OCM.Auth;OCM.Renamer")
set (ALL_DEPENDS 
	# Library
	stdlib inih
	# Programs
	Checker Auth Renamer
	# License
	${CMAKE_SOURCE_DIR}/LICENSE 
	${CMAKE_SOURCE_DIR}/NOTICE 
	# Template
	${CMAKE_SOURCE_DIR}/Templates/us-1.udp.ovpn 
	${CMAKE_SOURCE_DIR}/Templates/uk-2.udp.ovpn 
	${CMAKE_SOURCE_DIR}/Templates/ca-3.tcp.ovpn 
	${CMAKE_SOURCE_DIR}/Templates/jp-4.tcp.ovpn 
	${CMAKE_SOURCE_DIR}/Renamer/Template.ini 
)
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

set (TEMPLATES ${CMAKE_SOURCE_DIR}/Templates)
include_directories (".")

add_subdirectory ("inih")
add_subdirectory ("stdlib")

foreach (DIR ${SUBDIR})
	add_subdirectory ("${DIR}")
endforeach ()

# Organize directory structure

set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
# Program
install (TARGETS Checker Auth Renamer stdlib inih
		 RUNTIME DESTINATION .
		 LIBRARY DESTINATION .)
# License
install (FILES	"${CMAKE_SOURCE_DIR}/LICENSE"
				"${CMAKE_SOURCE_DIR}/NOTICE"
		 DESTINATION .)
# Template
install (DIRECTORY "${CMAKE_SOURCE_DIR}/Templates"
		 DESTINATION .)
install (FILES	"${CMAKE_SOURCE_DIR}/Renamer/Template.ini"
		 DESTINATION Templates)

# Install package

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Setup.nsi.in" "${CMAKE_CURRENT_BINARY_DIR}/Setup.nsi" @ONLY)
add_custom_command (
	OUTPUT OpenVPNConfigManagerSetup-${VERSION}.exe
	DEPENDS ${ALL_DEPENDS} ${CMAKE_BINARY_DIR}/Setup.nsi
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Generating NSIS Setup Wizard"
	COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR}
	COMMAND makensis "${CMAKE_BINARY_DIR}/Setup.nsi"
)

add_custom_command (
	OUTPUT OpenVPNConfigManagerSetup-${VERSION}.zip
	DEPENDS ${ALL_DEPENDS} deploy
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Generating program file package"
	COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR}
	COMMAND "C:/Program Files/7-Zip/7z.exe" a -tzip "OpenVPNConfigManagerSetup-${VERSION}.zip" "${CMAKE_INSTALL_PREFIX}/*"
)

add_custom_target (build DEPENDS OpenVPNConfigManagerSetup-${VERSION}.exe)

add_custom_target (
	deploy
	DEPENDS OpenVPNConfigManagerSetup-${VERSION}.exe
	COMMENT "Deploying ${PROJECT_NAME}"
	COMMAND ${CMAKE_BINARY_DIR}/OpenVPNConfigManagerSetup-${VERSION}.exe /S
)

add_custom_target (
	zip
	DEPENDS OpenVPNConfigManagerSetup-${VERSION}.zip
	COMMENT "Creating program file package"
)
