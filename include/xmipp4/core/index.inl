// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "index.hpp"

#include <type_traits>
#include <stdexcept>

namespace xmipp4 
{

namespace detail
{

template <typename I>
inline
typename std::enable_if<std::is_integral<I>::value && !std::is_signed<I>::value, std::size_t>::type
sanitize_index(I index, std::size_t size)
{
    if (index >= size)
    {
        throw std::out_of_range("Index exceeds or equals array length");
    }

    return static_cast<std::size_t>(index);
}

template <typename I>
inline
typename std::enable_if<std::is_integral<I>::value && std::is_signed<I>::value, std::size_t>::type
sanitize_index(I index, std::size_t length)
{
    std::size_t result;

    if (index < 0)
    {
        if (index < (-length))
        {
            throw std::out_of_range("Index exceeds negative array length");
        }

        result = static_cast<std::size_t>(index + length);
    }
    else
    {
        using unsigned_type = typename std::make_unsigned<I>::type;
        result = sanitize_index(static_cast<unsigned_type>(index), length);
    }

    return result;
}

template <typename I, I index>
inline
std::size_t sanitize_index(std::integral_constant<I, index>, std::size_t length)
{
    return sanitize_index(index, length);
}

} // namespace detail



template <typename I>
inline
std::size_t sanitize_index(I index, std::size_t length)
{
    return detail::sanitize_index(index, length);
}

} // namespace xmipp4
