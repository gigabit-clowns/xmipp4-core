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
 * @file lru_async_loader.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of lru_async_loader.hpp
 * @date 2024-10-25
 * 
 */

#include "lru_async_loader.hpp"

#include <xmipp4/core/image/reader.hpp>
#include <xmipp4/core/image/reader_manager.hpp>

namespace xmipp4
{
namespace image
{

std::unique_ptr<async_load_result> lru_async_loader::read_batch(span<const location> locations)
{
    auto first = locations.begin();
    while (first != locations.end()) 
    {
        const auto reader = m_cache.get_reader(first->get_filename());
        const auto position = first->get_position();
        if (position == location::no_position) 
        {
            reader->read(); // TODO
            ++first;
        }
        else
        {
            auto last = find_contiguous_location_run(first, locations.end());
            const std::size_t count = std::distance(first, last);
            reader->read_batch(position, count); // TODO
            first = last;
        }
    }
}

std::unique_ptr<async_load_result> lru_async_loader::read_single(const location &location)
{
    const auto reader = m_cache.get_reader(location.get_filename());
    const auto position = location.get_position();
    if (position == location::no_position) 
    {
        reader->read(); // TODO
    }
    else
    {
        reader->read_single(position); // TODO
    }
}

} // namespace image
} // namespace xmipp4
