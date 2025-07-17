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
abs(U x) noexcept;

template <typename I>
XMIPP4_CONSTEXPR
typename std::make_unsigned<typename std::enable_if<std::is_integral<I>::value && std::is_signed<I>::value, I>::type>::type
abs(I x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
abs(F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "abs.inl"
