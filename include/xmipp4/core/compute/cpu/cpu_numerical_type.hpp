// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../../numerical_type.hpp"
#include "../../fixed_float.hpp"
#include "../../platform/constexpr.hpp"

#include <complex>
#include <type_traits>
#include <cstddef>

namespace xmipp4
{
namespace compute
{

template <typename T>
struct cpu_numerical_type
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::unknown;
    }
};

template <>
struct cpu_numerical_type<std::int8_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::int8;
    }
};

template <>
struct cpu_numerical_type<std::uint8_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::uint8;
    }
};

template <>
struct cpu_numerical_type<std::int16_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::int16;
    }
};

template <>
struct cpu_numerical_type<std::uint16_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::uint16;
    }
};

template <>
struct cpu_numerical_type<std::int32_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::int32;
    }
};

template <>
struct cpu_numerical_type<std::uint32_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::uint32;
    }
};

template <>
struct cpu_numerical_type<std::int64_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::int64;
    }
};

template <>
struct cpu_numerical_type<std::uint64_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::uint64;
    }
};

template <>
struct cpu_numerical_type<float16_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::float16;
    }
};

template <>
struct cpu_numerical_type<float32_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::float32;
    }
};

template <>
struct cpu_numerical_type<float64_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::float64;
    }
};

template <>
struct cpu_numerical_type<std::complex<float16_t>>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::complex_float16;
    }
};

template <>
struct cpu_numerical_type<std::complex<float32_t>>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::complex_float32;
    }
};

template <>
struct cpu_numerical_type<std::complex<float64_t>>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::complex_float64;
    }
};

} // namespace compute
} // namespace xmipp4
