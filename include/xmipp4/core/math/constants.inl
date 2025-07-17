// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file constants.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of constants.hpp
 * @date 2024-04-15
 * 
 */

#include "constants.hpp"

namespace xmipp4
{
namespace math
{

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
pi() noexcept
{
    return 3.14159265358979323846L;
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
pi_2() noexcept
{
    return 1.57079632679489661923L;
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
pi_4() noexcept
{
    return 0.785398163397448309616L;
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
e() noexcept
{
    return 2.71828182845904523536L;
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sqrt2() noexcept
{
    return 1.41421356237309504880L;
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sqrt1_2() noexcept
{
    return 0.707106781186547524401L;
}

} // namespace math
} // namespace xmipp4
