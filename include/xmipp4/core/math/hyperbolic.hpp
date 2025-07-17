// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file hyperbolic.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides hyperbolic functions.
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
cosh(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sinh(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
tanh(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
acosh(F ratio) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
asinh(F ratio) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
atanh(F ratio) noexcept;

} // namespace math
} // namespace xmipp4

#include "hyperbolic.inl"
