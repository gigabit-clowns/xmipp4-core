// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/constexpr.hpp"

namespace xmipp4
{
namespace math
{

template <typename F>
F multiply_add(F x, F y, F z) noexcept;

template <typename F>
F mod(F num, F den) noexcept;

template <typename F>
F sign(F x) noexcept;

template <typename BidirIt, typename F>
XMIPP4_CONSTEXPR
F evaluate_polynomial(BidirIt first, BidirIt last, F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "arithmetic.inl"
