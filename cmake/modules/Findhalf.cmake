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

cmake_minimum_required(VERSION 3.12)

include(FindPackageHandleStandardArgs)

# Try to find the half library include dir
find_path(
    half_INCLUDE_DIR 
    NAMES half.hpp
    PATH_SUFFIXES half_float half
)

# Parse version from header file
if(half_INCLUDE_DIR)
    file(
        STRINGS "${half_INCLUDE_DIR}/half.hpp"
        half_VERSION_LINE
        REGEX "Version"
    )
    string(REGEX MATCH "Version ([0-9]*\.[0-9]*\.[0-9]*)" _ ${half_VERSION_LINE})
    set(half_VERSION ${CMAKE_MATCH_1})
endif()

# Define the package variables
find_package_handle_standard_args(half 
    REQUIRED_VARS half_INCLUDE_DIR
    VERSION_VAR half_VERSION
)

# Define the target
set(HALF_TARGET "half")
add_library(${HALF_TARGET} INTERFACE IMPORTED)
target_include_directories(${HALF_TARGET} INTERFACE ${half_INCLUDE_DIR})
