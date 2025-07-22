// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/constexpr.hpp"

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
F bspline0(F x) noexcept;

template <typename F>
F bspline1(F x) noexcept;

template <typename F>
F bspline2(F x) noexcept;

template <typename F>
F bspline3(F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "bspline.inl"
