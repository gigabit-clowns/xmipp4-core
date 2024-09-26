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
include(CMakePackageConfigHelpers)

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
	DEFINE_SYMBOL "XMIPP4_CORE_EXPORTING"
)
target_include_directories(
	${PROJECT_NAME} 
	PUBLIC 
	  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(
	${PROJECT_NAME} 
	PRIVATE
	VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
	VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
	VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
)
target_precompile_headers(
	${PROJECT_NAME} 
	PRIVATE
	${HEADERS}
)
target_link_libraries(${PROJECT_NAME} half)
if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()


if(MSVC)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DNOMINMAX)
endif()

# Install library's binary files and headers
install(
	TARGETS ${PROJECT_NAME} 
	EXPORT ${PROJECT_NAME}-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Create and install config files
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/config/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
	COMPATIBILITY SameMinorVersion
)

install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/cmake/modules/
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules
)
install(
  EXPORT ${PROJECT_NAME}-targets
  FILE ${PROJECT_NAME}-targets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(
  FILES 
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
