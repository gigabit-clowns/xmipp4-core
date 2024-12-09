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

include(FetchContent)

function(fetch_half)
    set(options)
    set(oneValueArgs VERSION)
    set(multiValueArgs)
    cmake_parse_arguments(PARSE_ARGV 0 arg
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
    )

	cmake_policy(SET CMP0135 NEW) # To avoid warnings
    set(NO_OP_COMMAND "${CMAKE_COMMAND} -E true")
    FetchContent_Declare(
        half
        URL https://kumisystems.dl.sourceforge.net/project/half/half/${arg_VERSION}/half-${arg_VERSION}.zip
        CONFIGURE_COMMAND ${NO_OP_COMMAND}
        BUILD_COMMAND ${NO_OP_COMMAND}
        INSTALL_COMMAND ${NO_OP_COMMAND}
    )
    FetchContent_MakeAvailable(half)

	set(half_INCLUDE_DIR ${half_SOURCE_DIR}/include)
    install(FILES ${half_INCLUDE_DIR}/half.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

	# Define the target
	add_library(half INTERFACE IMPORTED)
	target_include_directories(half INTERFACE ${half_INCLUDE_DIR})
endfunction()
