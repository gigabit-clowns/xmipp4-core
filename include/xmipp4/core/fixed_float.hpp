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
 * @file fixed_float.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Fixed width floating point types.
 * @date 2024-11-25
 * 
 */

#include <half.hpp>

namespace xmipp4
{

/**
 * @brief 16 bit floating point number representation.
 * 
 */
using float16_t = half_float::half;
static_assert(sizeof(float16_t) == 2, "float16_t should be 2 bytes long");

/**
 * @brief 32 bit floating point number representation.
 * 
 */
using float32_t = float;
static_assert(sizeof(float32_t) == 4, "float32_t should be 4 bytes long");

/**
 * @brief 64 bit floating point number representation.
 * 
 */
using float64_t = double;
static_assert(sizeof(float64_t) == 8, "float64_t should be 8 bytes long");

} // namespace xmipp4
