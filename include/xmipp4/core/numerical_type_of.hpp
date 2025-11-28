// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "numerical_type.hpp"
#include "fixed_float.hpp"
#include "platform/constexpr.hpp"

#include <complex>
#include <cstdint>

namespace xmipp4
{

template <typename T>
struct numerical_type_of
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::unknown;
    }
};

template <>
struct numerical_type_of<std::int8_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::int8;
    }
};

template <>
struct numerical_type_of<std::uint8_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::uint8;
    }
};

template <>
struct numerical_type_of<std::int16_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::int16;
    }
};

template <>
struct numerical_type_of<std::uint16_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::uint16;
    }
};

template <>
struct numerical_type_of<std::int32_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::int32;
    }
};

template <>
struct numerical_type_of<std::uint32_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::uint32;
    }
};

template <>
struct numerical_type_of<std::int64_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::int64;
    }
};

template <>
struct numerical_type_of<std::uint64_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::uint64;
    }
};

template <>
struct numerical_type_of<float16_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::float16;
    }
};

template <>
struct numerical_type_of<float32_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::float32;
    }
};

template <>
struct numerical_type_of<float64_t>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::float64;
    }
};

template <>
struct numerical_type_of<std::complex<float16_t>>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::complex_float16;
    }
};

template <>
struct numerical_type_of<std::complex<float32_t>>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::complex_float32;
    }
};

template <>
struct numerical_type_of<std::complex<float64_t>>
{
    XMIPP4_CONSTEXPR
    static numerical_type value() noexcept
    {
        return numerical_type::complex_float64;
    }
};

} // namespace xmipp4
