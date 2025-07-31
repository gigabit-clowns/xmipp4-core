// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
F erf(F x) noexcept;

template <typename F>
F erfc(F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "erf.inl"
