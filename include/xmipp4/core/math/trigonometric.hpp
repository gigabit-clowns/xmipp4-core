// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <utility>
#include <type_traits>

#include "../platform/constexpr.hpp"

namespace xmipp4
{
namespace math
{

template <typename F>
F cos(F angle) noexcept;

template <typename F>
F sin(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, void>::type
sincos(F angle, F &y, F &x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, std::pair<F, F>>::type
sincos(F angle) noexcept;

template <typename F>
F tan(F angle) noexcept;

template <typename F>
F acos(F ratio) noexcept;

template <typename F>
F asin(F ratio) noexcept;

template <typename F>
F atan(F ratio) noexcept;

template <typename F>
F atan2(F y, F x) noexcept;

template <typename I>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_integral<I>::value, int>::type
cos_pi(I x) noexcept;

} // namespace math
} // namespace xmipp4

#include "trigonometric.inl"
