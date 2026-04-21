// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <limits>
#include <cstdint>

namespace xmipp4
{

/**
 * @brief 16 bit floating point number representation.
 * 
 * As there is no standardized half-width float type in C/C++ use a
 * placeholder.
 */
struct float16_t { std::uint16_t bits; };
static_assert(sizeof(float16_t) == 2, "float16_t should be 2 bytes long");

/**
 * @brief 16 bit "brain" floating point number representation.
 * 
 * As there is no standardized bfloat16 type in C/C++ use a placeholder.
 */
struct bfloat16_t { std::uint16_t bits; };
static_assert(sizeof(float16_t) == 2, "bfloat16_t should be 2 bytes long");

/**
 * @brief 32 bit floating point number representation.
 */
using float32_t = float;
static_assert(sizeof(float32_t) == 4, "float32_t should be 4 bytes long");

/**
 * @brief 64 bit floating point number representation.
 */
using float64_t = double;
static_assert(sizeof(float64_t) == 8, "float64_t should be 8 bytes long");

} // namespace xmipp4
