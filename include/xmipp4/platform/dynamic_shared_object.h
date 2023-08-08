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
 * @file dynamic_shared_object.h
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides API definition macros for selectively exporting
 * to the final binary file.
 * @date 2023-03-25
 * 
 */

#include "gcc_attributes.h"
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
 * @def XMIPP4_LOCAL
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