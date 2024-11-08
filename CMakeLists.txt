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
project(
  xmipp4-core
	VERSION 0.1.0 
	LANGUAGES C CXX
)

# Include CMake packages
include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Declare build options
option(XMIPP4_CORE_BUILD_DOC "Build documentation" OFF)
option(XMIPP4_CORE_LINT_CLANG_TIDY "Lint with clang-tidy" OFF)
option(XMIPP4_CORE_PRECOMPILED_HEADERS "Use precompiled headers for faster compilation times" ON)

# Set the module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Only build docs if it is the main project
if(XMIPP4_CORE_BUILD_DOC)
	add_subdirectory(doc)
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
add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(
	${PROJECT_NAME} 
	PROPERTIES 
		SOVERSION ${PROJECT_VERSION}
		CXX_STANDARD 20
		DEFINE_SYMBOL "XMIPP4_CORE_EXPORTING"
)
target_include_directories(
	${PROJECT_NAME} 
	PUBLIC 
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(
	${PROJECT_NAME} 
	PRIVATE
		VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
		VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
		VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
)

# Link dependencies
if(MSVC)
	target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32) # Winsock
endif()

# Set up clang-tidy
if(XMIPP4_CORE_LINT_CLANG_TIDY)
	find_program(CLANG_TIDY_EXE clang-tidy REQUIRED)
	set_target_properties(
		${PROJECT_NAME} 
		PROPERTIES 
			CXX_CLANG_TIDY ${CLANG_TIDY_EXE}
	)
elseif(XMIPP4_CORE_PRECOMPILED_HEADERS)
	# Precompiler headers do not work with clang-tidy
	target_precompile_headers(
		${PROJECT_NAME} 
		PRIVATE
			${HEADERS}
	)
endif()

# Enable all warnings during compilation
if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4)
	target_compile_options(${PROJECT_NAME} PRIVATE /wd4996) # getenv warning
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Disable min/max macros in windows
if(MSVC)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DNOMINMAX)
endif()

# Install library's binary files and headers
install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}-config
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/xmipp4/
)
install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ 
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
  	EXPORT ${PROJECT_NAME}-config
  	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Only build tests if it is the main project
if(BUILD_TESTING)
	add_subdirectory(tests)
endif()
