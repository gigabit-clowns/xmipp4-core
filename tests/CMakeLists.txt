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

find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

# Test excecutable name
set(TESTS_NAME "${PROJECT_NAME}_tests")

# Register all source and header files
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# Add the test executable
add_executable(
    ${TESTS_NAME}
    ${TEST_SOURCES}
)
set_target_properties(
	${TESTS_NAME} PROPERTIES 
	CMAKE_CXX_STANDARD 20
	CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic"
)
target_link_libraries(${TESTS_NAME} PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)

# Discover tests
catch_discover_tests(${TESTS_NAME})