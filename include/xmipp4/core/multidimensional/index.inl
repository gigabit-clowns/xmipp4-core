// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "index.hpp"

#include <type_traits>
#include <sstream>
#include <stdexcept>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
inline
typename std::enable_if<is_index<T>::value, std::size_t>::type
sanitize_index(T index, std::size_t extent)
{
    auto result = static_cast<std::ptrdiff_t>(index);
    
    if (result < 0)
    {
        if (result < -static_cast<std::ptrdiff_t>(extent))
        {
            std::ostringstream oss;
            oss << "Reverse index " << result 
                << " is out of bounds for extent " << extent;
            throw std::out_of_range(oss.str());
        }

        result += extent;
    }
    else if (result >= static_cast<std::ptrdiff_t>(extent))
    {
        std::ostringstream oss;
        oss << "Index " << result
            << " is out of bounds for extent " << extent;
        throw std::out_of_range(oss.str());
    }

    return static_cast<std::size_t>(result);
}

} // namespace multidimensional
} // namespace xmipp4
