// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file factorial.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides factorial and gamma functions.
 * @date 2024-04-15
 * 
 */

#include "../platform/constexpr.hpp"

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename U>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_unsigned<U>::value, U>::type
factorial(U x) noexcept;

template <typename T, typename U>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_unsigned<U>::value, T>::type
large_factorial(U x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
gamma(F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "factorial.inl"
