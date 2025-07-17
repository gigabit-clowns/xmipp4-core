// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file erf.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides the error function (erf).
 * @date 2024-04-15
 * 
 */

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
