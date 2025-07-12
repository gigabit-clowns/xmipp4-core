#***************************************************************************
# Authors:     Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307  USA
#
#  All comments concerning this program package may be sent to the
#  e-mail address 'xmipp@cnb.csic.es'
# ***************************************************************************

cmake_minimum_required(VERSION 3.16)

# Define the project
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
project(
  xmipp4-core
	VERSION ${PROJECT_VERSION}
	LANGUAGES C CXX
)

# Declare build options
option(XMIPP4_CORE_BUILD_DOC "Build documentation" OFF)
option(XMIPP4_CORE_LINT_CLANG_TIDY "Lint with clang-tidy" OFF)
option(XMIPP4_CORE_PRECOMPILED_HEADERS "Use precompiled headers for faster compilation times" ON)
option(XMIPP4_CORE_BUILD_SPDLOG "Build and install spdlog instead of using a pre-installed binary" ON)
option(XMIPP4_CORE_BUILD_HALF "Build and install half instead of using a pre-installed binary" ON)

# Set the CMake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set(XMIPP4_CORE_LIBRARY_TARGET "xmipp4-core")

# Include CMake packages
include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(fetch_ghc_filesystem)
include(fetch_half)
include(fetch_spdlog)

# Only build docs if it is the main project
if(XMIPP4_CORE_BUILD_DOC)
	add_subdirectory(doc)
endif()

# Find dependencies
find_package(Threads REQUIRED)
fetch_ghc_filesystem(VERSION 1.5.14)

if (XMIPP4_CORE_BUILD_SPDLOG)
	fetch_spdlog(VERSION 1.15.0)
else()
	find_package(spdlog REQUIRED)
endif()

if (XMIPP4_CORE_BUILD_HALF)
	fetch_half(VERSION 2.2.1)
else()
	find_package(half 2 REQUIRED)
	install(
		FILES 
			${PROJECT_SOURCE_DIR}/cmake/modules/Findhalf.cmake
		DESTINATION 
			${CMAKE_INSTALL_LIBDIR}/cmake/${XMIPP4_CORE_LIBRARY_TARGET}/modules
	)
endif()


# Find all source and header files
file(
	GLOB_RECURSE 
	SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)
file(
	GLOB_RECURSE 
	HEADERS 
		${CMAKE_CURRENT_SOURCE_DIR}/include/*.h 
		${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)
file(
	GLOB_RECURSE 
	INLINE_SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/include/*.inl
)

# Create the shared library
add_library(${XMIPP4_CORE_LIBRARY_TARGET} SHARED ${SOURCES})
set_target_properties(
	${XMIPP4_CORE_LIBRARY_TARGET} 
	PROPERTIES 
		SOVERSION ${PROJECT_VERSION}
		CXX_STANDARD 20
		DEFINE_SYMBOL "XMIPP4_CORE_EXPORTING"
)
target_include_directories(
	${XMIPP4_CORE_LIBRARY_TARGET} 
	PUBLIC 
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(
	${XMIPP4_CORE_LIBRARY_TARGET} 
	PRIVATE
		VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
		VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
		VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
)

# Link dependencies
if(WIN32)
	target_link_libraries(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PRIVATE ws2_32 # Winsock
	) 
endif()

target_link_libraries(
	${XMIPP4_CORE_LIBRARY_TARGET} 
	PUBLIC
		Threads::Threads
		spdlog::spdlog_header_only
		half
	PRIVATE
		ghc_filesystem # Eventually replace by std::filesystem (C++17)
		${CMAKE_DL_LIBS}
)


# Set up clang-tidy
if(XMIPP4_CORE_LINT_CLANG_TIDY)
	find_program(CLANG_TIDY_EXE clang-tidy REQUIRED)
	set_target_properties(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PROPERTIES 
			CXX_CLANG_TIDY ${CLANG_TIDY_EXE}
	)
elseif(XMIPP4_CORE_PRECOMPILED_HEADERS)
	# Precompiler headers do not work with clang-tidy
	target_precompile_headers(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PRIVATE
			${HEADERS}
	)
endif()

# Enable all warnings during compilation
if(MSVC)
	target_compile_options(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PRIVATE 
			/W4
			/wd4996 # getenv warning
	)
else()
	target_compile_options(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PRIVATE 
			-Wall 
			-Wextra 
			-Wpedantic
	)
endif()

# Disable min/max macros in windows
if(MSVC)
	target_compile_definitions(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PUBLIC 
			NOMINMAX
	)
endif()

# Disable builtin functions in MinGW
if(MINGW)
	target_compile_definitions(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PUBLIC 
			XMIPP4_NO_BUILTIN
	)
endif()

# Install library's binary files and headers
install(
	TARGETS ${XMIPP4_CORE_LIBRARY_TARGET}
	EXPORT ${XMIPP4_CORE_LIBRARY_TARGET}-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ 
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Create and install CMake config files
if(WIN32)
	set(XMIPP4_PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_BINDIR}/xmipp4-plugins)
else()
	set(XMIPP4_PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/xmipp4-plugins)
endif()
set(XMIPP4_PLUGIN_INSTALL_DIR ${XMIPP4_PLUGIN_INSTALL_DIR} CACHE PATH "Directory where the xmipp4 plugins are installed")

configure_package_config_file(
  	${PROJECT_SOURCE_DIR}/cmake/config/config.cmake.in
  	${CMAKE_CURRENT_BINARY_DIR}/${XMIPP4_CORE_LIBRARY_TARGET}-config.cmake
  	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${XMIPP4_CORE_LIBRARY_TARGET}
)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/${XMIPP4_CORE_LIBRARY_TARGET}-config-version.cmake
	COMPATIBILITY SameMinorVersion
)
install(
  	EXPORT ${XMIPP4_CORE_LIBRARY_TARGET}-targets
  	FILE ${XMIPP4_CORE_LIBRARY_TARGET}-targets.cmake
  	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${XMIPP4_CORE_LIBRARY_TARGET}
)
install(
  	FILES 
		${CMAKE_CURRENT_BINARY_DIR}/${XMIPP4_CORE_LIBRARY_TARGET}-config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/${XMIPP4_CORE_LIBRARY_TARGET}-config-version.cmake
  	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${XMIPP4_CORE_LIBRARY_TARGET}
)

# Only build tests if it is the main project
if(BUILD_TESTING)
	add_subdirectory(tests)
endif()
