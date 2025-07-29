// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/constexpr.hpp"

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename U>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_unsigned<U>::value, U>::type
square(U x) noexcept;

template <typename I>
XMIPP4_CONSTEXPR
typename std::make_unsigned<typename std::enable_if<std::is_integral<I>::value && std::is_signed<I>::value, I>::type>::type
square(I x) noexcept;

template <typename F>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
square(F x) noexcept;

template <typename F>
F pow(F base, F exponent) noexcept;

template <typename F>
F sqrt(F x) noexcept;

template <typename F>
F rsqrt(F x) noexcept;

template <typename F>
F cbrt(F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "power.inl"
