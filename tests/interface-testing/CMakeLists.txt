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

set(INTERFACE_TESTS_NAME ${PROJECT_NAME}-interface-testing)

# Register all source and header files
file(
	GLOB_RECURSE 
	SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

# Create the library
add_library(${INTERFACE_TESTS_NAME} SHARED ${SOURCES})
set_target_properties(
	${INTERFACE_TESTS_NAME} PROPERTIES 
	CXX_STANDARD 20
)
target_link_libraries(
	${INTERFACE_TESTS_NAME} 
	PRIVATE 
		${PROJECT_NAME} 
		Catch2::Catch2
)
target_include_directories(
	${INTERFACE_TESTS_NAME} 
	PUBLIC 
	  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
