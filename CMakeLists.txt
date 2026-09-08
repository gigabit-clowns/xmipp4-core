cmake_minimum_required(VERSION 3.18)

# Without this CMAKE_MSVC_DEBUG_INFORMATION_FORMAT is silently ignored.
if(POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
endif()

# Define the project
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
project(
	rexlib
	VERSION ${PROJECT_VERSION}
	LANGUAGES C CXX
)

# Add the CMake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Include CMake packages
include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(fetch_half)
include(fetch_spdlog)
include(fetch_boost)
include(fetch_pocketfft)
include(fetch_eigen)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------

option(REXLIB_INSTALL "Generate rexlib's install rules" ON)
option(REXLIB_BUILD_DOC "Build documentation" OFF)
option(REXLIB_ENABLE_COVERAGE "Enable code coverage" OFF)
option(REXLIB_REGISTER_TESTS_PER_BINARY "Register one test per test executable instead of one per Catch2 case" OFF)

# Lets a consumer that embeds rexlib drop its tests without dropping its own.
option(REXLIB_BUILD_TESTING "Build rexlib's tests" ${BUILD_TESTING})

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------

find_package(Threads REQUIRED)
fetch_boost(VERSION 1.92.0)
fetch_half(VERSION 2.2.1)
fetch_spdlog(VERSION 1.17.0)
fetch_pocketfft(COMMIT c90e55b3d529f8efa40ed01a20de22405f45fc65)
fetch_eigen(VERSION 5.0.1)

# ---------------------------------------------------------------------------
# The package
# ---------------------------------------------------------------------------

# The default goes through a variable of its own: assigning to the cache
# entry's own name first would leave a normal variable shadowing it, and the
# value given on the command line would be overwritten on every configure.
#
# STRING rather than PATH, because this is a destination relative to the
# install prefix: cmake turns a relative -D value of a PATH entry into an
# absolute one, which would place the plugins outside the prefix.
if(WIN32)
	set(REXLIB_PLUGINS_DIR_DEFAULT ${CMAKE_INSTALL_BINDIR}/rexlib-plugins)
else()
	set(REXLIB_PLUGINS_DIR_DEFAULT ${CMAKE_INSTALL_LIBDIR}/rexlib-plugins)
endif()
set(
	REXLIB_PLUGINS_INSTALL_DIR ${REXLIB_PLUGINS_DIR_DEFAULT}
	CACHE STRING "Directory where the rexlib plugins are installed"
)

# Same helper the installed package config offers, so that a plugin built
# inside a superbuild installs itself exactly as an out-of-tree one does.
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config/rexlib-plugin.cmake)

# Project-wide default for every target created below. The sources are
# written to C++14 and reach anything newer through the REXLIB_* feature
# macros, so this is a preference and not a requirement: a compiler without
# C++17 still builds them.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

# Add source directory
add_subdirectory(src)

# Only build tests if both this project and the build as a whole want them
if(BUILD_TESTING AND REXLIB_BUILD_TESTING)
	add_subdirectory(tests)
endif()

# Only build docs if requested
if(REXLIB_BUILD_DOC)
	add_subdirectory(doc)
endif()
