// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file exponential.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides multiple exponential and logarithmic functions.
 * @date 2024-04-15
 * 
 */

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
exp(F exponent) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
exp2(F exponent) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
exp10(F exponent) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
log(F x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
log2(F x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
log10(F x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
logn(F n, F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "exponential.inl"
