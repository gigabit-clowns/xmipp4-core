// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/constexpr.hpp"

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
XMIPP4_CONSTEXPR
F pi() noexcept;

template <typename F>
XMIPP4_CONSTEXPR
F pi_2() noexcept;

template <typename F>
XMIPP4_CONSTEXPR
F pi_4() noexcept;

template <typename F>
XMIPP4_CONSTEXPR
F e() noexcept;

template <typename F>
XMIPP4_CONSTEXPR
F sqrt2() noexcept;

template <typename F>
XMIPP4_CONSTEXPR
F sqrt1_2() noexcept;

} // namespace math
} // namespace xmipp4

#include "constants.inl"
