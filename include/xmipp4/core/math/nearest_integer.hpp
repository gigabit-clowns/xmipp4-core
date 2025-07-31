// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
F trunc(F x) noexcept;

template <typename F>
F floor(F x) noexcept;

template <typename F>
F ceil(F x) noexcept;

template <typename F>
F round(F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "nearest_integer.inl"
