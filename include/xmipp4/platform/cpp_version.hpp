#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file cpp_version.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Macro definitions for detecting C++ version
 * @date 2023-08-08
 * 
 * This file provides definitions for determining 
 * the C++ version support
 * 
 */

#define XMIPP4_CPLUSPLUS (__cplusplus)
#define XMIPP4_CPLUSPLUS11 (201103L)
#define XMIPP4_CPLUSPLUS14 (201402L)
#define XMIPP4_CPLUSPLUS17 (201703L)
#define XMIPP4_CPLUSPLUS20 (202002L)
#define XMIPP4_CPLUSPLUS23 (999999L) /*TBD*/

#define XMIPP4_HAS_CPP11 (XMIPP4_CPLUSPLUS >= XMIPP4_CPLUSPLUS11)
#define XMIPP4_HAS_CPP14 (XMIPP4_CPLUSPLUS >= XMIPP4_CPLUSPLUS14)
#define XMIPP4_HAS_CPP17 (XMIPP4_CPLUSPLUS >= XMIPP4_CPLUSPLUS17)
#define XMIPP4_HAS_CPP20 (XMIPP4_CPLUSPLUS >= XMIPP4_CPLUSPLUS20)
#define XMIPP4_HAS_CPP23 (XMIPP4_CPLUSPLUS >= XMIPP4_CPLUSPLUS23)
