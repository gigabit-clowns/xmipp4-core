// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <limits>
#include <cstdint>

#include "../platform/attributes.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4
{

/**
 * @brief 16 bit floating point number representation.
 *
 * As there is no standardized half-width float type in C/C++ use a
 * placeholder.
 */
class XMIPP4_CORE_API float16_t
{
public:
	float16_t() noexcept;
	float16_t(float value) noexcept;
	float16_t(const float16_t &other) = default;
	float16_t(float16_t &&other) = default;
	~float16_t() = default;

	float16_t& operator=(const float16_t &other) = default;
	float16_t& operator=(float16_t &&other) = default;

	operator float() const noexcept;

private:
	XMIPP4_UNUSED std::uint16_t m_bits; 
};
static_assert(sizeof(float16_t) == 2, "float16_t should be 2 bytes long");

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
