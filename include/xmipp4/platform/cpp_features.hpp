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
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Macro definitions for feature testing C++ compiler and 
 * standard library
 * @date 2024-02-16
 * @see https://en.cppreference.com/w/cpp/feature_test
 * 
 * This file provides definitions for testing support of C++ features 
 * available for the compiler and standard library.
 * 
 */

#include "cpp_version.hpp"

#if XMIPP4_HAS_CPP20
# include <version>
#endif

/**
 * @brief Macro to test if a C++ feature is available
 * @param feature Name of the feature (without the __cpp_ prefix)
 * @param version Requested standard version as YYYYMM
 * @see https://en.cppreference.com/w/cpp/feature_test
 * 
 */
#define XMIPP4_HAS_CPP_FEATURE(feature, version) \
    (defined(__cpp_##feature) && (__cpp_##feature >= version))

/**
 * @def XMIPP4_HAS_CONSTEXPR 
 * @brief Tests if constexpr is supported by the compiler
 * 
 */
#if XMIPP4_HAS_CPP_FEATURE(constexpr, 200704L)
# define XMIPP4_HAS_CONSTEXPR 1
#elif XMIPP4_HAS_CPP11
# if defined(__clang__) && defined(__apple_build_version__)
#  if __apple_build_version__ >= 10000000 
#   define XMIPP4_HAS_CONSTEXPR 1
#  else
#   define XMIPP4_HAS_CONSTEXPR 0
#  endif
# elif defined(__GNUC__)
#  if __GNUC__ >= 5 
#   define XMIPP4_HAS_CONSTEXPR 1
#  else
#   define XMIPP4_HAS_CONSTEXPR 0
#  endif
# elif defined(_MSC_VER)
#  if _MSC_VER >= 1900
#   define XMIPP4_HAS_CONSTEXPR 1
#  else
#   define XMIPP4_HAS_CONSTEXPR 0
#  endif
# else
#  pragma message ("Unknown compiler. Assuming constexpr support")
#  define CONSTEXPR_SUPPORTED 1
# endif
#else
# define XMIPP4_HAS_CONSTEXPR 0
#endif



#define XMIPP4_HAS_IF_CONSTEXPR XMIPP4_HAS_CPP_FEATURE(if_constexpr, 201606L)
#define XMIPP4_HAS_LIB_BITOPS XMIPP4_HAS_CPP_FEATURE(lib_bitops, 201907)
#define XMIPP4_HAS_LIB_POW2 XMIPP4_HAS_CPP_FEATURE(lib_pow2, 202002)
#define XMIPP4_HAS_LIB_EXECUTION XMIPP4_HAS_CPP_FEATURE(lib_execution, 201603)