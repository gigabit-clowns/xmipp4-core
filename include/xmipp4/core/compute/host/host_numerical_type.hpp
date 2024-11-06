#pragma once

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
 * @file host_numerical_type.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Numerical type conversions for host
 * @date 2024-11-06
 * 
 */

#include "../numerical_type.hpp"
#include "../../platform/constexpr.hpp"

#include <complex>
#include <cstddef>

namespace xmipp4
{
namespace compute
{

template <typename T>
struct host_numerical_type
{
    static numerical_type value() noexcept
    {
        return numerical_type::unknown;
    }
};

template <>
struct host_numerical_type<std::int8_t>
{
    static numerical_type value() noexcept
    {
        return numerical_type::int8;
    }
};

template <>
struct host_numerical_type<std::uint8_t>
{
    static numerical_type value() noexcept
    {
        return numerical_type::uint8;
    }
};

template <>
struct host_numerical_type<std::int16_t>
{
    static numerical_type value() noexcept
    {
        return numerical_type::int16;
    }
};

template <>
struct host_numerical_type<std::uint16_t>
{
    static numerical_type value() noexcept
    {
        return numerical_type::uint16;
    }
};

template <>
struct host_numerical_type<std::int32_t>
{
    static numerical_type value() noexcept
    {
        return numerical_type::int32;
    }
};

template <>
struct host_numerical_type<std::uint32_t>
{
    static numerical_type value() noexcept
    {
        return numerical_type::uint32;
    }
};

template <>
struct host_numerical_type<std::int64_t>
{
    static numerical_type value() noexcept
    {
        return numerical_type::int64;
    }
};

template <>
struct host_numerical_type<std::uint64_t>
{
    static numerical_type value() noexcept
    {
        return numerical_type::uint64;
    }
};

template <>
struct host_numerical_type<float>
{
    static numerical_type value() noexcept
    {
        return numerical_type::float32; // FIXME not guaranteed
    }
};

template <>
struct host_numerical_type<double>
{
    static numerical_type value() noexcept
    {
        return numerical_type::float64; // FIXME not guaranteed
    }
};

template <>
struct host_numerical_type<std::complex<float>>
{
    static numerical_type value() noexcept
    {
        return numerical_type::complex_float32;
    }
};

template <>
struct host_numerical_type<std::complex<double>>
{
    static numerical_type value() noexcept
    {
        return numerical_type::complex_float64;
    }
};

} // namespace compute
} // namespace xmipp4

