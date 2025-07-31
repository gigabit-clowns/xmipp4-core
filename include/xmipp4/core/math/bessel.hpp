// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
F cylindrical_bessel_j0(F x) noexcept;

template <typename F>
F cylindrical_bessel_j1(F x) noexcept;

template <typename F>
F cylindrical_bessel_jn(int n, F x) noexcept;



template <typename F>
F cylindrical_bessel_y0(F x) noexcept;

template <typename F>
F cylindrical_bessel_y1(F x) noexcept;

template <typename F>
F cylindrical_bessel_yn(int n, F x) noexcept;



template <typename F>
F cylindrical_bessel_i0(F x) noexcept;

template <typename F>
F cylindrical_bessel_i1(F x) noexcept;

template <typename F>
F cylindrical_bessel_in(int n, F x) noexcept;



template <typename F>
F cylindrical_bessel_k0(F x) noexcept;

template <typename F>
F cylindrical_bessel_k1(F x) noexcept;

template <typename F>
F cylindrical_bessel_kn(int n, F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "bessel.inl"
