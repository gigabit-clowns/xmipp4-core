// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "c_attributes.h"
#include "operating_system.h"

/**
 * @def XMIPP4_IMPORT
 * @brief Declares that the function should be imported from a shared object
 * 
 */
#if defined(XMIPP4_WINDOWS)
    #if XMIPP4_HAS_GCC_ATTRIBUTE(dllimport)
        #define XMIPP4_IMPORT XMIPP4_GCC_ATTRIBUTE(dllimport)
    #else
        #define XMIPP4_IMPORT __declspec(dllimport)
    #endif
#elif XMIPP4_HAS_GCC_ATTRIBUTE(visibility)
    #define XMIPP4_IMPORT XMIPP4_GCC_ATTRIBUTE(visibility("default"))
#else
    #define XMIPP4_IMPORT
#endif

/**
 * @def XMIPP4_EXPORT
 * @brief Declares that the function should be exported to a shared object
 * 
 */
#if defined(XMIPP4_WINDOWS)
    #if XMIPP4_HAS_GCC_ATTRIBUTE(dllexport)
        #define XMIPP4_EXPORT XMIPP4_GCC_ATTRIBUTE(dllexport)
    #else
        #define XMIPP4_EXPORT __declspec(dllexport)
    #endif
#elif XMIPP4_HAS_GCC_ATTRIBUTE(visibility)
    #define XMIPP4_EXPORT XMIPP4_GCC_ATTRIBUTE(visibility("default"))
#else
    #define XMIPP4_EXPORT
#endif

/**
 * @def XMIPP4_LOCAL
 * @brief Declares that the function is only used used locally at the current shared object
 * 
 */
#if defined(XMIPP4_WINDOWS)
    #define XMIPP4_LOCAL
#elif XMIPP4_HAS_GCC_ATTRIBUTE(visibility)
    #define XMIPP4_LOCAL XMIPP4_GCC_ATTRIBUTE(visibility("hidden"))
#else
    #define XMIPP4_LOCAL
#endif

/**
 * @def XMIPP4_CORE_API
 * @brief Declares that the function is part of the public core API of XMIPP4
 * 
 * The functions declared as public core API will be exported to the shared object. 
 * 
 */
#if defined(XMIPP4_CORE_EXPORTING)
    #define XMIPP4_CORE_API XMIPP4_EXPORT
#else
    #define XMIPP4_CORE_API XMIPP4_IMPORT
#endif