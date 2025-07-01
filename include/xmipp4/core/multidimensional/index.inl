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
        if (result < -extent)
        {
            std::ostringstream oss;
            oss << "Reverse index " << result 
                << " is out of bounds for extent " << extent;
            throw std::out_of_range(oss.str());
        }

        result += extent;
    }
    else if (result >= extent)
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
