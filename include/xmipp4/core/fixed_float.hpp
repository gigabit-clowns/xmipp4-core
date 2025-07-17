// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
