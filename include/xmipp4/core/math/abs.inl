// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file abs.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of abs.hpp
 * @date 2024-04-15
 * 
 */

#include "abs.hpp"

#include "../platform/builtin.h"

#include <cmath>

namespace xmipp4
{
namespace math
{

template <typename U>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_unsigned<U>::value, U>::type
abs(U x) noexcept
{
    return x; // Nothing to do
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR
typename std::make_unsigned<typename std::enable_if<std::is_integral<I>::value && std::is_signed<I>::value, I>::type>::type
abs(I x) noexcept
{
    return x < 0 ? -x : x;
}

namespace detail
{

inline float abs(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(fabsf)
    return XMIPP4_BUILTIN(fabsf)(x);
#else
    return fabsf(x);
#endif
}

inline double abs(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(fabs)
    return XMIPP4_BUILTIN(fabs)(x);
#else
    return fabs(x);
#endif
}

inline long double abs(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(fabsl)
    return XMIPP4_BUILTIN(fabsl)(x);
#else
    return fabsl(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
abs(F x) noexcept
{
    return detail::abs(x);
}

} // namespace math
} // namespace xmipp4
