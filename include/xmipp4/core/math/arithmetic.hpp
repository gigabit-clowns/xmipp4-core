// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file arithmetic.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides various arithmetic functions.
 * @date 2024-04-15
 * 
 */

#include "../platform/constexpr.hpp"

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
multiply_add(F x, F y, F z) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
mod(F num, F den) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sign(F x) noexcept;

template <typename BidirIt, typename F>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
evaluate_polynomial(BidirIt first, BidirIt last, F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "arithmetic.inl"
