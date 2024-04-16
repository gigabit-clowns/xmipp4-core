/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file arithmetic.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of arithmetic.hpp
 * @date 2024-04-15
 * 
 */

#include "arithmetic.hpp"

#include "../platform/builtin.h"

#include <cmath>

namespace xmipp4
{
namespace math
{

namespace detail
{

inline float multiply_add(float x, float y, float z) noexcept
{
#if XMIPP4_HAS_BUILTIN(fmaf)
    return XMIPP4_BUILTIN(fmaf)(x, y, z);
#else
    return fmaf(x, y, z);
#endif
}

inline double multiply_add(double x, double y, double z) noexcept
{
#if XMIPP4_HAS_BUILTIN(fma)
    return XMIPP4_BUILTIN(fma)(x, y, z);
#else
    return ::fma(x, y, z);
#endif
}

inline long double multiply_add(long double x, long double y, long double z) noexcept
{
#if XMIPP4_HAS_BUILTIN(fmal)
    return XMIPP4_BUILTIN(fmal)(x, y, z);
#else
    return fmal(x, y, z);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
multiply_add(F x, F y, F z) noexcept
{
    return detail::multiply_add(x, y, z);
}





namespace detail
{

inline float mod(float num, float den) noexcept
{
#if XMIPP4_HAS_BUILTIN(fmodf)
    return XMIPP4_BUILTIN(fmodf)(num, den);
#else
    return fmodf(num, den);
#endif
}

inline double mod(double num, double den) noexcept
{
#if XMIPP4_HAS_BUILTIN(fmod)
    return XMIPP4_BUILTIN(fmod)(num, den);
#else
    return ::fmod(num, den);
#endif
}

inline long double mod(long double num, long double den) noexcept
{
#if XMIPP4_HAS_BUILTIN(fmodl)
    return XMIPP4_BUILTIN(fmodl)(num, den);
#else
    return fmodl(num, den);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
mod(F num, F den) noexcept
{
    return detail::mod(num, den);
}



template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sign(F x) noexcept
{
    switch (std::fpclassify(x))
    {
    case FP_NAN:
        return x;

    case FP_ZERO:
        return F(0);

    default: // Determine sign
        return x<0 ? F(-1) : F(1); 
    }
}

template <typename BidirIt, typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
evaluate_polynomial(BidirIt first, BidirIt last, F x) noexcept
{
    F y = 0;
    if (std::distance(first, last) > 0)
    {
        // Evaluate using Horner's method
        // Initialize with the last element
        auto ite = std::make_reverse_iterator(last);
        y = *ite;
        ++ite;

        for(; ite != std::make_reverse_iterator(first); ++ite)
        {
            y = multiply_add(y, x, *ite);
        } 
    }

    return y;
}

} // namespace math
} // namespace xmipp4
