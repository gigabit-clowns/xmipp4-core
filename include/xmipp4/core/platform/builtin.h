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
 * @file builtin.h
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Macro definitions to check compiler builtin functions
 * @date 2023-08-08
 * 
 */

/**
 * @brief Declaration name of a builtin function
 * 
 */
#define XMIPP4_BUILTIN(fun) __builtin_##fun

/**
 * @def XMIPP4_HAS_BUILTIN(fun)
 * @brief Checks if a particular builtin function is provided
 * 
 * When no way of checking availability is provided, it 
 * defaults to false.
 * 
 */
#if defined(__has_builtin) && defined(__GNUC__)
    #define XMIPP4_HAS_BUILTIN(fun) __has_builtin(__builtin_##fun)
#elif defined(XMIPP4_DOC_BUILD)
    #define XMIPP4_HAS_BUILTIN(fun) 1
#else
    #define XMIPP4_HAS_BUILTIN(fun) 0
#endif
