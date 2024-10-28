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

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/dummy_plugin.cpp)
add_library(dummy_plugin MODULE ${SOURCES})
set_target_properties(
	dummy_plugin
	PROPERTIES 
		CXX_STANDARD 20
		DEFINE_SYMBOL "XMIPP4_DUMMY_PLUGIN_EXPORTING"
)
target_link_libraries(
	dummy_plugin
	PRIVATE
		xmipp4-core
)
