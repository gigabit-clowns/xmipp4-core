// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
erf(F x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
erfc(F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "erf.inl"
