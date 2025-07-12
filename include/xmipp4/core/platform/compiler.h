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
 * @file compiler.h
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides macros for detecting the compiler
 * @date 2025-07-12
 * 
 */

/**
 * @def XMIPP4_CLANG
 * @brief Defined when building with Clang
 * 
 */

/**
 * @def XMIPP4_GCC
 * @brief Defined when building with GNU C/C++ compiler
 * 
 */

/**
 * @def XMIPP4_MSVC
 * @brief Defined when building with Microsoft Visual C++
 * 
 */

#if defined(_MSC_VER) && !defined(__llvm__)
#endif

#if defined(__clang__)
    #define XMIPP4_CLANG 1
#elif defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
    #define XMIPP4_INTEL_COMPILER 1
#elif defined(__GNUC__) || defined(__GNUG__)
    #define XMIPP4_GCC 1
#elif defined(_MSC_VER)
    #define XMIPP4_MSVC 1
#else
    #pragma message ("Could not determine the compiler")
#endif

/**
 * @def XMIPP4_MINGW
 * @brief Defined when building with MinGW.
 * 
 * Unlike the former compiler macros, this definition is not exclusive and
 * it may be present together with other definitions.
 * 
 */
#if defined(__MINGW32__) || defined(__MINGW64__)
    #define XMIPP4_MINGW 1
#endif
