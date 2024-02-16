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
 * @file cpp_features.hpp
 * @author Mikel Iceta Tena (miceta@cnb.csic.es)
 * @brief Macro definitions for feature testing CPP STDLIB
 * @date 2024-02-16
 * 
 * This file provides definitions for CPP features present and
 * supported in the host machine
 * 
 */

#include "cpp_version.hpp"

#if defined(__cpp_lib_int_pow2) && (__cpp_lib_int_pow2 >= XMIPP4_CPLUSPLUS20)
    #define XMIPP4_LIB_INT_POW2
#endif

#if defined(__cpp_lib_execution) && (__cpp_lib_execution >= XMIPP4_CPLUSPLUS17)
    #define XMIPP_LIB_EXECUTION
#endif
