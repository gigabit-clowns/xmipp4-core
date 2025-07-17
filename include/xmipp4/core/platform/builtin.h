// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file builtin.h
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Macro definitions to check compiler builtin functions
 * @date 2023-08-08
 * 
 */

/**
 * @brief Declaration name of a builtin function
 * 
 */
#define XMIPP4_BUILTIN(fun) __builtin_##fun

/**
 * @def XMIPP4_HAS_BUILTIN(fun)
 * @brief Checks if a particular builtin function is provided
 * 
 * When no way of checking availability is provided, it 
 * defaults to false.
 * 
 */
#if defined(__has_builtin) && !defined(XMIPP4_NO_BUILTIN)
    #define XMIPP4_HAS_BUILTIN(fun) __has_builtin(__builtin_##fun)
#elif defined(XMIPP4_DOC_BUILD)
    #define XMIPP4_HAS_BUILTIN(fun) 1
#else
    #define XMIPP4_HAS_BUILTIN(fun) 0
#endif
