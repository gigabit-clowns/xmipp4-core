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
 * @file operating_system.h
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides macros for detecting the target OS
 * @date 2023-08-08
 * 
 */

/**
 * @def XMIPP4_WINDOWS
 * @brief Defined if the target is Windows (32bit or 64bit)
 * 
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define XMIPP4_WINDOWS 1
#endif

/**
 * @def XMIPP4_APPLE
 * @brief Defined if the target is MacOS
 * 
 */
#if defined(__APPLE__) || defined(__MACH__)
    #define XMIPP4_APPLE 1
#endif

/**
 * @def XMIPP4_UNIX
 * @brief Defined if the target is Unix-like, including Linux and MacOS
 * 
 */
#if defined(__unix__) || defined(__unix) || defined(XMIPP4_APPLE)
    #define XMIPP4_UNIX 1
#endif

/**
 * @def XMIPP4_LINUX
 * @brief Defined if the target is Linux
 * 
 */
#if defined(__linux__) || defined(__linux) || defined(__gnu_linux__)
    #define XMIPP4_LINUX 1
#endif

/**
 * @def XMIPP4_BSD
 * @brief Defined if the target is BSD
 * 
 */
#if defined(BSD)
    #define XMIPP4_BSD 1
#endif

/**
 * @def XMIPP4_POSIX
 * @brief Defined if the target is POSIX
 * 
 */
#if defined(_POSIX_VERSION) || defined(XMIPP4_UNIX)
    #define XMIPP4_POSIX 1
#endif
