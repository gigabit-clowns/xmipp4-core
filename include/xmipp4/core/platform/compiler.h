// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
 * @def XMIPP4_ICC
 * @brief Defined when building with Intel Compiler
 * 
 */

/**
 * @def XMIPP4_ICX
 * @brief Defined when building with Intel LLVM based Compiler
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

#if defined(__clang__)
    #define XMIPP4_CLANG 1
#elif defined(__INTEL_COMPILER)
    #define XMIPP4_ICC 1
#elif defined(__INTEL_LLVM_COMPILER)
    #define XMIPP4_ICX 1
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
