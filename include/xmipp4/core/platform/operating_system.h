// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
