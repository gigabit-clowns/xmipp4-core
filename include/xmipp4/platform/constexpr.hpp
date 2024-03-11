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
 * @file constexpr.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Macro definitions for version dependant constexpr
 * @date 2023-08-08
 * 
 * This file provides definitions for using constexpr when
 * supported
 * 
 */

#include "cpp_features.hpp"

#if XMIPP4_HAS_CONSTEXPR || defined(XMIPP4_DOC_BUILD)
    #define XMIPP4_CONSTEXPR constexpr
    #define XMIPP4_CONST_CONSTEXPR constexpr
    #define XMIPP4_INLINE_CONSTEXPR constexpr
    #define XMIPP4_INLINE_CONST_CONSTEXPR constexpr
#else
    #define XMIPP4_CONSTEXPR
    #define XMIPP4_CONST_CONSTEXPR const
    #define XMIPP4_INLINE_CONSTEXPR inline
    #define XMIPP4_INLINE_CONST_CONSTEXPR inline const
#endif

#if XMIPP4_HAS_IF_CONSTEXPR
    #define XMIPP4_IF_CONSTEXPR XMIPP4_CONSTEXPR
#else
    #define XMIPP4_IF_CONSTEXPR
#endif

// C++11
#if XMIPP4_HAS_CPP11
    #define XMIPP4_CONSTEXPR_CPP11 XMIPP4_CONSTEXPR
    #define XMIPP4_CONST_CONSTEXPR_CPP11 XMIPP4_CONST_CONSTEXPR
    #define XMIPP4_INLINE_CONSTEXPR_CPP11 XMIPP4_INLINE_CONSTEXPR
    #define XMIPP4_INLINE_CONST_CONSTEXPR_CPP11 XMIPP4_INLINE_CONST_CONSTEXPR

#else
    #define XMIPP4_CONSTEXPR_CPP11
    #define XMIPP4_CONST_CONSTEXPR_CPP11 const
    #define XMIPP4_INLINE_CONSTEXPR_CPP11 inline
    #define XMIPP4_INLINE_CONST_CONSTEXPR_CPP11 inline const
#endif

// C++14
#if XMIPP4_HAS_CPP14
    #define XMIPP4_CONSTEXPR_CPP14 XMIPP4_CONSTEXPR
    #define XMIPP4_CONST_CONSTEXPR_CPP14 XMIPP4_CONST_CONSTEXPR
    #define XMIPP4_INLINE_CONSTEXPR_CPP14 XMIPP4_INLINE_CONSTEXPR
    #define XMIPP4_INLINE_CONST_CONSTEXPR_CPP14 XMIPP4_INLINE_CONST_CONSTEXPR
#else
    #define XMIPP4_CONSTEXPR_CPP14
    #define XMIPP4_CONST_CONSTEXPR_CPP14 const
    #define XMIPP4_INLINE_CONSTEXPR_CPP14 inline
    #define XMIPP4_INLINE_CONST_CONSTEXPR_CPP14 inline const
#endif

// C++17
#if XMIPP4_HAS_CPP17
    #define XMIPP4_CONSTEXPR_CPP17 XMIPP4_CONSTEXPR
    #define XMIPP4_CONST_CONSTEXPR_CPP17 XMIPP4_CONST_CONSTEXPR
    #define XMIPP4_INLINE_CONSTEXPR_CPP17 XMIPP4_INLINE_CONSTEXPR
    #define XMIPP4_INLINE_CONST_CONSTEXPR_CPP17 XMIPP4_INLINE_CONST_CONSTEXPR
#else
    #define XMIPP4_CONSTEXPR_CPP17
    #define XMIPP4_CONST_CONSTEXPR_CPP17 const
    #define XMIPP4_INLINE_CONSTEXPR_CPP17 inline
    #define XMIPP4_INLINE_CONST_CONSTEXPR_CPP17 inline const
#endif

// C++20
#if XMIPP4_HAS_CPP20
    #define XMIPP4_CONSTEXPR_CPP20 XMIPP4_CONSTEXPR
    #define XMIPP4_CONST_CONSTEXPR_CPP20 XMIPP4_CONST_CONSTEXPR
    #define XMIPP4_INLINE_CONSTEXPR_CPP20 XMIPP4_INLINE_CONSTEXPR
    #define XMIPP4_INLINE_CONST_CONSTEXPR_CPP20 XMIPP4_INLINE_CONST_CONSTEXPR
#else
    #define XMIPP4_CONSTEXPR_CPP20
    #define XMIPP4_CONST_CONSTEXPR_CPP20 const
    #define XMIPP4_INLINE_CONSTEXPR_CPP20 inline
    #define XMIPP4_INLINE_CONST_CONSTEXPR_CPP20 inline const
#endif

// C++23
#if XMIPP4_HAS_CPP23
    #define XMIPP4_CONSTEXPR_CPP23 XMIPP4_CONSTEXPR
    #define XMIPP4_CONST_CONSTEXPR_CPP23 XMIPP4_CONST_CONSTEXPR
    #define XMIPP4_INLINE_CONSTEXPR_CPP23 XMIPP4_INLINE_CONSTEXPR
    #define XMIPP4_INLINE_CONST_CONSTEXPR_CPP23 XMIPP4_INLINE_CONST_CONSTEXPR
#else
    #define XMIPP4_CONSTEXPR_CPP23
    #define XMIPP4_CONST_CONSTEXPR_CPP23 const
    #define XMIPP4_INLINE_CONSTEXPR_CPP23 inline
    #define XMIPP4_INLINE_CONST_CONSTEXPR_CPP23 inline const
#endif