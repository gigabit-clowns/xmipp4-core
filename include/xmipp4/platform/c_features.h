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