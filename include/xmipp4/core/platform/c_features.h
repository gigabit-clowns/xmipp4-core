// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @def XMIPP4_HAS_C_FEATURE
 * @brief Check if a feature is supported by the compiler
 * @see https://en.cppreference.com/w/cpp/feature_test
 * 
 */
#if defined(__has_feature)
    #define XMIPP4_HAS_C_FEATURE(feature) __has_feature(feature)
#else
    #define XMIPP4_HAS_C_FEATURE(feature) 0
#endif