cmake_minimum_required(VERSION 3.16)

# Define the project
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
project(
	xmipp4-core
	VERSION ${PROJECT_VERSION}
	LANGUAGES C CXX
)

# Declare configuration options
option(XMIPP4_CORE_BUILD_DOC "Build documentation" OFF)
option(XMIPP4_CORE_BUILD_SPDLOG "Build and install spdlog instead of using a pre-installed binary" ON)
option(XMIPP4_CORE_BUILD_HALF "Build and install half instead of using a pre-installed binary" ON)

# Add the CMake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Include CMake packages
include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(fetch_half)
include(fetch_spdlog)
include(fetch_boost)

# Find dependencies
find_package(Threads REQUIRED)
fetch_boost(VERSION 1.89.0)

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
			${CMAKE_INSTALL_LIBDIR}/cmake/xmipp4-core/modules
	)
endif()


# Set project-wide compilation options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

# Add source directory
add_subdirectory(src)

# Only build tests if it is the main project
if(BUILD_TESTING)
	add_subdirectory(tests)
endif()

# Only build docs if requested
if(XMIPP4_CORE_BUILD_DOC)
	add_subdirectory(doc)
endif()
