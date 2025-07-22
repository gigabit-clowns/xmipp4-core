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
F log(F x) noexcept;

template <typename F>
F log2(F x) noexcept;

template <typename F>
F log10(F x) noexcept;

template <typename F>
F logn(F n, F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "exponential.inl"
