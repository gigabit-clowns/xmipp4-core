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

# Configure CMake
cmake ^
	-B build ^
	-G "%CMAKE_GENERATOR%" ^
	-DCMAKE_INSTALL_PREFIX="%LIBRARY_PREFIX%" ^
	-DCMAKE_BUILD_TYPE=Release ^
	-DBUILD_DOC=OFF

IF %ERRORLEVEL% NEQ 0 exit %ERRORLEVEL%

# Build
cmake ^
	--build build ^
	--config Release

IF %ERRORLEVEL% NEQ 0 exit %ERRORLEVEL%

# Install
cmake ^
	--install build ^
	--config Release

exit %ERRORLEVEL%