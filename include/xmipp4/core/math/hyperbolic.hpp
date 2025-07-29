// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
F cosh(F angle) noexcept;

template <typename F>
F sinh(F angle) noexcept;

template <typename F>
F tanh(F angle) noexcept;

template <typename F>
F acosh(F ratio) noexcept;

template <typename F>
F asinh(F ratio) noexcept;

template <typename F>
F atanh(F ratio) noexcept;

} // namespace math
} // namespace xmipp4

#include "hyperbolic.inl"
