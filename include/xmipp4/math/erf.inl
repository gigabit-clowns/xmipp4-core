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
 * @file erf.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of erf.hpp
 * @date 2024-04-15
 * 
 */

#include "erf.hpp"

#include "../platform/builtin.h"

#include <cmath>

namespace xmipp4
{
namespace math
{

namespace detail
{

inline float erf(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erff)
    return XMIPP4_BUILTIN(erff)(x);
#else
    return erff(x);
#endif
}

inline double erf(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erf)
    return XMIPP4_BUILTIN(erf)(x);
#else
    return ::erf(x);
#endif
}

inline long double erf(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erfl)
    return XMIPP4_BUILTIN(erfl)(x);
#else
    return erfl(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
erf(F x) noexcept
{
    return detail::erf(x);
}





namespace detail
{

inline float erfc(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erfcf)
    return XMIPP4_BUILTIN(erfcf)(x);
#else
    return erfcf(x);
#endif
}

inline double erfc(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erfc)
    return XMIPP4_BUILTIN(erfc)(x);
#else
    return ::erfc(x);
#endif
}

inline long double erfc(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erfcl)
    return XMIPP4_BUILTIN(erfcl)(x);
#else
    return erfcl(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
erfc(F x) noexcept
{
    return detail::erfc(x);
}

} // namespace math
} // namespace xmipp4
