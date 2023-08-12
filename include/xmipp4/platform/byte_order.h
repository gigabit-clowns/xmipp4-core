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
 * @file byte_order.h
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Macro definitions for determining byte ordering (endianess)
 * @date 2023-08-12
 */

/**
 * @brief Constant representing little endian byte ordering
 * 
 */
#define XMIPP4_BYTE_ORDER_LITTLE_ENDIAN 1

/**
 * @brief Constant representing big endian byte ordering
 * 
 */
#define XMIPP4_BYTE_ORDER_BIG_ENDIAN 2


/**
 * @def XMIPP4_BYTE_ORDER
 * @brief Indicates the byte ordering used by the system
 * 
 * Detection based on rapidjson code
 */

//Detect with GCC 4.6's macro.
#if defined(__BYTE_ORDER__)
    #if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        #define XMIPP4_BYTE_ORDER XMIPP4_BYTE_ORDER_LITTLE_ENDIAN
    #elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        #define XMIPP4_BYTE_ORDER XMIPP4_BYTE_ORDER_BIG_ENDIAN
    #else
        #error "Unknown byte order"
    #endif

// Detect with _LITTLE_ENDIAN and _BIG_ENDIAN macro.
#elif defined(_LITTLE_ENDIAN)
    #define XMIPP4_BYTE_ORDER XMIPP4_BYTE_ORDER_LITTLE_ENDIAN
#elif defined(_BIG_ENDIAN)
    #define XMIPP4_BYTE_ORDER XMIPP4_BYTE_ORDER_BIG_ENDIAN

// Failed detecting 
#else
    #error "Could not determine byte ordering"
#endif


/**
 * @def XMIPP4_FLOAT_BYTE_ORDER
 * @brief Indicates the byte ordering used by the Floating
 * Point Unit of the system
 * 
 */
#if defined(__FLOAT_WORD_ORDER__)
    #if (__FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        #define XMIPP4_FLOAT_BYTE_ORDER XMIPP4_BYTE_ORDER_LITTLE_ENDIAN
    #elif (__FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__)
        #define XMIPP4_FLOAT_BYTE_ORDER XMIPP4_BYTE_ORDER_BIG_ENDIAN
    #else
        #error "Unknown float byte order"
    #endif

#else
    #warning "Could not determine float byte ordering. Defaulting to system byte order"
    #define XMIPP4_FLOAT_BYTE_ORDER XMIPP4_BYTE_ORDER
#endif
