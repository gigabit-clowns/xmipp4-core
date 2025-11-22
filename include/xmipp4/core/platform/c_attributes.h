// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @brief GCC attribute declaration
 * @see https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
 * 
 */
#define XMIPP4_GCC_ATTRIBUTE(attr) __attribute__((attr))

/**
 * @def XMIPP4_HAS_GCC_ATTRIBUTE(attr)
 * @brief Checks if a particular gcc attribute is supported
 * 
 * When no way of checking availability is provided, it 
 * defaults to false
 * 
 */
#if defined(__has_attribute)
	#define XMIPP4_HAS_GCC_ATTRIBUTE(attr) __has_attribute(attr)
#elif defined(XMIPP4_DOC_BUILD)
	#define XMIPP4_HAS_GCC_ATTRIBUTE(attr) 1
#else
	#define XMIPP4_HAS_GCC_ATTRIBUTE(attr) 0
#endif
