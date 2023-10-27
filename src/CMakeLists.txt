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

include(GNUInstallDirs)

# Find dependencies
find_package(half 2.1 REQUIRED)

# Register all source and header files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE INLINE_SOURCES ${PROJECT_SOURCE_DIR}/include/*.inl)

# Create the shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(
	${PROJECT_NAME} PROPERTIES 
	SOVERSION ${PROJECT_VERSION}
	CXX_STANDARD 20
	CXX_FLAGS "-Wall -Wextra -Wpedantic"
	DEFINE_SYMBOL "XMIPP4_CORE_EXPORTING"
)
target_include_directories(
	${PROJECT_NAME} 
	PUBLIC 
	${PROJECT_SOURCE_DIR}/include/
)
target_compile_definitions(
	${PROJECT_NAME} 
	PRIVATE
	XMIPP4_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
	XMIPP4_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
	XMIPP4_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
)
target_precompile_headers(
	${PROJECT_NAME} 
	PRIVATE
	${HEADERS}
)
target_link_libraries(${PROJECT_NAME} half)

# Install library's binary files and headers
install(
	TARGETS ${PROJECT_NAME} 
	LIBRARY 
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
	DIRECTORY include/ 
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)