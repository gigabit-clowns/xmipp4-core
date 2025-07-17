// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file constants.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of common mathematical constants.
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
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
pi() noexcept;

template <typename F>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
pi_2() noexcept;

template <typename F>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
pi_4() noexcept;

template <typename F>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
e() noexcept;

template <typename F>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sqrt2() noexcept;

template <typename F>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sqrt1_2() noexcept;

} // namespace math
} // namespace xmipp4

#include "constants.inl"
