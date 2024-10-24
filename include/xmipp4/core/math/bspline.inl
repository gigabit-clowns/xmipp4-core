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
 * @file bspline.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of bspline.hpp
 * @date 2024-10-24
 * 
 */

#include "bspline.hpp"

#include "abs.hpp"

namespace xmipp4
{
namespace math
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
bspline0(F x) noexcept
{
    const auto ax = std::abs(x);
    return ax < F(0.5) ? F(1.0) : F(0.0);
}

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
bspline1(F x) noexcept
{
    const auto ax = std::abs(x);
    return ax < F(1.0) ? F(1.0) - ax : F(0.0);
}

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
bspline2(F x) noexcept
{
    F result = 0.0;

    const auto ax = abs(x);
    if (ax < F(0.5))
    {
        result = F(0.75) - ax*ax;
    }
    else if (ax < F(1.5))
    {
        result = 0.5*ax*ax - F(1.5)*ax + F(9.0/8.0);
    }

    return result;
}

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
bspline3(F x) noexcept
{
    F result = 0.0;

    const auto ax = abs(x);
    if(ax < F(1))
    {
        result = F(2.0/3.0) + F(0.5)*ax*ax*(ax - F(2.0));
    }
    else if(ax < F(2))
    {
        const auto y = F(2.0) - ax;
        result = F(1.0/6.0)*y*y*y;
    }

    return result;
}

} // namespace math
} // namespace xmipp4
