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
 * @file attributes.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Macro definitions for supported attributes
 * @date 2023-08-08
 * 
 * This file declares a variety of C++ attributes through
 * macros. When an attribute is not supported, first it
 * tries to provide an alternative though compiler intrinsic
 * macros. If no alternative is available, a dummy declaration
 * is provided. This allows the programmer to use the attributes 
 * through macro definitions with no compatibility check, and
 * then its declaration will handle the best option available.
 */

#include "cpp_attributes.hpp"
#include "c_attributes.h"
#include "builtin.h"



/**
 * @def XMIPP4_NORETURN
 * @brief Indicates that the function does not return
 * 
 */
#if XMIPP4_HAS_CPP_ATTRIBUTE(noreturn)
    #define XMIPP4_NORETURN XMIPP4_CPP_ATTRIBUTE(noreturn)
#elif XMIPP4_HAS_GCC_ATTRIBUTE(noreturn)
    #define XMIPP4_NORETURN XMIPP4_GCC_ATTRIBUTE(noreturn)
#else
    #define XMIPP4_NORETURN
#endif

/**
 * @def XMIPP4_FALLTHROUGH
 * @brief Used to explicitly avoid warnings when a break condition
 * is not desired in a switch statement
 * 
 */
#if XMIPP4_HAS_CPP_ATTRIBUTE(fallthrough)
    #define XMIPP4_FALLTHROUGH XMIPP4_CPP_ATTRIBUTE(fallthrough)
#elif XMIPP4_HAS_GCC_ATTRIBUTE(fallthrough)
    #define XMIPP4_FALLTHROUGH XMIPP4_GCC_ATTRIBUTE(fallthrough)
#else
    #define XMIPP4_FALLTHROUGH
#endif

/**
 * @def XMIPP4_DEPRECATED(reason)
 * @brief Indicates that a function is deprecated providing a reason
 * 
 */
#if XMIPP4_HAS_CPP_ATTRIBUTE(deprecated)
    #if XMIPP4_HAS_CPP14
        #define XMIPP4_DEPRECATED(reason) XMIPP4_CPP_ATTRIBUTE(deprecated(reason))
    #else
        #define XMIPP4_DEPRECATED(reason) XMIPP4_CPP_ATTRIBUTE(deprecated)
    #endif
#elif XMIPP4_HAS_GCC_ATTRIBUTE(deprecated)
    #define XMIPP4_DEPRECATED XMIPP4_GCC_ATTRIBUTE(deprecated)
#else
    #define XMIPP4_DEPRECATED(reason)
#endif

/**
 * @def XMIPP4_NODISCARD
 * @brief Indicates that the return value should be used
 * 
 */
#if XMIPP4_HAS_CPP_ATTRIBUTE(nodiscard)
    #define XMIPP4_NODISCARD XMIPP4_CPP_ATTRIBUTE(nodiscard)
#elif XMIPP4_HAS_GCC_ATTRIBUTE(warn_unused_result)
    #define XMIPP4_NODISCARD XMIPP4_GCC_ATTRIBUTE(warn_unused_result)
#else
    #define XMIPP4_NODISCARD
#endif

/**
 * @def XMIPP4_NODISCARD_MESSAGE(reason)
 * @brief Indicates that the return value should be used, proving a message
 * 
 */
#if XMIPP4_HAS_CPP_ATTRIBUTE(nodiscard)
    #if XMIPP4_HAS_CPP20
        #define XMIPP4_NODISCARD_MESSAGE(reason) XMIPP4_CPP_ATTRIBUTE(nodiscard(reason))
    #else
        #define XMIPP4_NODISCARD_MESSAGE(reason) XMIPP4_CPP_ATTRIBUTE(nodiscard)
    #endif
#elif XMIPP4_HAS_GCC_ATTRIBUTE(warn_unused_result)
    #define XMIPP4_NODISCARD_MESSAGE(reason) XMIPP4_GCC_ATTRIBUTE(warn_unused_result)
#else
    #define XMIPP4_NODISCARD_MESSAGE(reason)
#endif

/**
 * @def XMIPP4_NO_UNIQUE_ADDRESS
 * @brief Indicates that the empty class does not need to have
 * a unique address and instead it can share space with other
 * variables
 * 
 */
#if XMIPP4_HAS_CPP_ATTRIBUTE(no_unique_address)
    #define XMIPP4_NO_UNIQUE_ADDRESS XMIPP4_CPP_ATTRIBUTE(no_unique_address)
#else
    #define XMIPP4_NO_UNIQUE_ADDRESS
#endif


/**
 * @def XMIPP4_ASSUME(expr)
 * @brief Assume that a particular expression evaluates to true
 * for compiler optimizations
 * 
 */
#if XMIPP4_HAS_CPP_ATTRIBUTE(assume)
    #define XMIPP4_ASSUME(expr) XMIPP4_CPP_ATTRIBUTE(assume(expr))
#elif XMIPP4_HAS_GCC_ATTRIBUTE(assume)
    #define XMIPP4_ASSUME(expr) XMIPP4_GCC_ATTRIBUTE(assume(expr))
#elif XMIPP4_HAS_BUILTIN(__builtin_assume)
    #define XMIPP4_ASSUME(expr) __builtin_assume(expr)
#elif defined(__assume)
    #define XMIPP4_ASSUME(expr) __assume(expr)
#else
    #define XMIPP4_ASSUME(expr)
#endif

/**
 * @def XMIPP4_LIKELY
 * @brief Indicates to the compiler that a particular branch is likely
 * 
 */
#if XMIPP4_HAS_CPP_ATTRIBUTE(likely)
    #define XMIPP4_LIKELY XMIPP4_CPP_ATTRIBUTE(likely)
#else
    #define XMIPP4_LIKELY
#endif

/**
 * @def XMIPP4_UNLIKELY
 * @brief Indicates to the compiler that a particular branch is unlikely
 * 
 */
#if XMIPP4_HAS_CPP_ATTRIBUTE(unlikely)
    #define XMIPP4_UNLIKELY XMIPP4_CPP_ATTRIBUTE(unlikely)
#else
    #define XMIPP4_UNLIKELY
#endif
