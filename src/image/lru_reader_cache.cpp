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
 * @file lru_reader_cache.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of lru_reader_cache.hpp
 * @date 2024-10-25
 * 
 */

#include "lru_reader_cache.hpp"

#include <xmipp4/core/image/reader_manager.hpp>

namespace xmipp4
{
namespace image
{

lru_reader_cache::lru_reader_cache(const reader_manager &reader_manager,
                                   std::size_t max_open )
    : m_reader_manager(reader_manager)
    , m_max_open(max_open)
{
}

const reader& lru_reader_cache::get_reader(const std::string &path)
{
    reader *result;

    const auto ite = m_reader_paths.find(path);
    if (ite != m_reader_paths.end()) 
    {
        // Bring the reader to the front of the list
        m_open_readers.splice(
            m_open_readers.cbegin(), 
            m_open_readers, 
            ite->second
        );

        result = ite->second->second.get();
    }
    else
    {
        // Reader does not exist, create it
        const auto pos = m_open_readers.emplace(
            m_open_readers.cbegin(), 
            path, 
            m_reader_manager.get().create_reader(path)
        );

        m_reader_paths.emplace(path, pos);

        result = pos->second.get();
    }

    // Delete the least recently used reader if we have too many open
    if (m_open_readers.size() > m_max_open) 
    {
        m_reader_paths.erase(m_open_readers.back().first);
        m_open_readers.pop_back();
    }

    return *result;
}

} // namespace image
} // namespace xmipp4
