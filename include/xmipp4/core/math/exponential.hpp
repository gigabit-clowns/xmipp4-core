// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
F exp(F exponent) noexcept;

template <typename F>
F exp2(F exponent) noexcept;

template <typename F>
F exp10(F exponent) noexcept;

template <typename F>
F expm1(F exponent) noexcept;

template <typename F>
F log(F x) noexcept;

template <typename F>
F log2(F x) noexcept;

template <typename F>
F log10(F x) noexcept;

template <typename F>
F logn(F n, F x) noexcept;

template <typename F>
F log1p(F x) noexcept;

template <typename F>
F log_add(F log_x, F log_y) noexcept;

} // namespace math
} // namespace xmipp4

#include "exponential.inl"
