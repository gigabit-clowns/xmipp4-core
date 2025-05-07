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

#include <unordered_map>
#include <list>
#include <utility>

namespace xmipp4
{
namespace image
{

class caching_batch_reader::implementation
{
public:
    implementation(const reader_manager &reader_manager, 
                   std::size_t max_open )
        : m_reader_manager(reader_manager)
        , m_max_open(max_open)
    {
    }

    ~implementation() = default;

    reader& get_file_reader(const std::string &path)
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

private:
    using key_value_pair = std::pair<const std::string, std::shared_ptr<reader>>;

    std::reference_wrapper<const reader_manager> m_reader_manager;
    std::size_t m_max_open;
    std::list<key_value_pair> m_open_readers;
    std::unordered_map<std::string, std::list<key_value_pair>::iterator> m_reader_paths;

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
        auto last = find_contiguous_location_run(first, locations.end());
        auto &reader = m_impl->get_file_reader(first->get_filename());
        // TODO

    }
}

} // namespace image
} // namespace xmipp4
