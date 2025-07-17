// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file c_features.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Macro definitions for feature testing C compiler and 
 * @date 2024-03-06
 * @see https://en.cppreference.com/w/cpp/feature_test
 * 
 * This file provides definitions for testing support of C features 
 * available for the compiler
 * 
 */

/**
 * @def XMIPP4_HAS_C_FEATURE
 * @brief Check if a feature is supported by the compiler
 * 
 */
#if defined(__has_feature)
    #define XMIPP4_HAS_C_FEATURE(feature) __has_feature(feature)
#else
    #define XMIPP4_HAS_C_FEATURE(feature) 0
#endif