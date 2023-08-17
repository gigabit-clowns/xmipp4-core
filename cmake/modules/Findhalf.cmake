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

find_path(half_INCLUDE_DIR NAMES half.hpp)
message(${half_INCLUDE_DIR})

set(half_VERSION 2.1.0) #TODO

# Define the target
add_library(half INTERFACE IMPORTED)
target_include_directories(half INTERFACE ${half_INCLUDE_DIR})

find_package_handle_standard_args(half 
    REQUIRED_VARS half_INCLUDE_DIR
    VERSION_VAR half_VERSION
)