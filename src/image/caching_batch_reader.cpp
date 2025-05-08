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
 * @file caching_batch_reader.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of caching_batch_reader.hpp
 * @date 2024-10-25
 * 
 */

#include <xmipp4/core/image/caching_batch_reader.hpp>

#include <xmipp4/core/image/reader.hpp>
#include <xmipp4/core/image/reader_manager.hpp>

#include "lru_reader_cache.hpp"

namespace xmipp4
{
namespace image
{

class caching_batch_reader::implementation
{
public:
    implementation(const reader_manager &reader_manager, 
                   std::size_t max_open )
        : m_cache(reader_manager, max_open)
    {
    }

    ~implementation() = default;

    std::shared_ptr<const reader> get_reader(const std::string &path)
    {
        return m_cache.get_reader(path);
    }

private:
    lru_reader_cache m_cache;

};



caching_batch_reader::caching_batch_reader(const reader_manager &readers, 
                                           std::size_t max_open )
    : m_impl(readers, max_open)
{
}

caching_batch_reader::caching_batch_reader(caching_batch_reader&& other) noexcept = default;

caching_batch_reader::~caching_batch_reader() = default;

caching_batch_reader& 
caching_batch_reader::operator=(caching_batch_reader&& other) noexcept = default;

void caching_batch_reader::read_batch(span<const location> locations)
{
    auto first = locations.begin();
    while (first != locations.end()) 
    {
        const auto reader = m_impl->get_reader(first->get_filename());
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

} // namespace image
} // namespace xmipp4
