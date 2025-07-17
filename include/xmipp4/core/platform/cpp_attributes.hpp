// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @brief C++ attribute declaration
 * @see https://en.cppreference.com/w/cpp/language/attributes
 * 
 */
#define XMIPP4_CPP_ATTRIBUTE(attr) [[attr]]

/**
 * @def XMIPP4_HAS_CPP_ATTRIBUTE(attr)
 * @brief Checks if a particular C++ attribute is supported
 * 
 * When no way of checking availability is provided, it 
 * defaults to false
 * 
 */
#if defined(__has_cpp_attribute)
    #define XMIPP4_HAS_CPP_ATTRIBUTE(attr) __has_cpp_attribute(attr)
#elif defined(XMIPP4_DOC_BUILD)
    #define XMIPP4_HAS_CPP_ATTRIBUTE(attr) 1
#else
    #define XMIPP4_HAS_CPP_ATTRIBUTE(attr) 0
#endif
