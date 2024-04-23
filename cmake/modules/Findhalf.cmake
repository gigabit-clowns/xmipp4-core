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
include(FetchContent)
include(GNUInstallDirs)

# Try to find the half library include dir
find_path(
    half_INCLUDE_DIR 
    NAMES half.hpp
    PATH_SUFFIXES half_float half
)

if (half_INCLUDE_DIR)
    # Parse version from header file
    file(
        STRINGS "${half_INCLUDE_DIR}/half.hpp"
        half_VERSION_LINE
        REGEX "Version"
    )
    string(REGEX MATCH "Version ([0-9]*\.[0-9]*\.[0-9]*)" _ ${half_VERSION_LINE})
    set(half_VERSION ${CMAKE_MATCH_1})
else()
    # Half not found. Obtain it from source
    set(half_VERSION 2.2.0)
    cmake_policy(SET CMP0135 NEW) # To avoid warnings
    FetchContent_Declare(
        half
        URL https://kumisystems.dl.sourceforge.net/project/half/half/${half_VERSION}/half-${half_VERSION}.zip
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
    )
    FetchContent_GetProperties(half)
    if(NOT half_POPULATED)
        FetchContent_Populate(half)
    endif()

    # Define the taget
    set(half_INCLUDE_DIR ${half_SOURCE_DIR}/include)
    install(DIRECTORY ${half_INCLUDE_DIR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

# Define the package variables
find_package_handle_standard_args(half 
    REQUIRED_VARS half_INCLUDE_DIR
    VERSION_VAR half_VERSION
)

# Define the target
add_library(half INTERFACE IMPORTED)
target_include_directories(half INTERFACE ${half_INCLUDE_DIR})
