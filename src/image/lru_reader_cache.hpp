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
 * @file lru_reader_cache.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::lru_reader_cache class
 * @date 2025-05-07
 * 
 */

#include <cstddef>
#include <unordered_map>
#include <list>
#include <string>
#include <memory>

namespace xmipp4 
{
namespace image
{

class reader;
class reader_manager;

/**
 * @brief Class to open and close readers on a Least Recently Used basis
 * 
 */
class lru_reader_cache
{
public:
    lru_reader_cache(const reader_manager &reader_manager,
                     std::size_t max_open );

    lru_reader_cache(const lru_reader_cache &other) = default;
    lru_reader_cache(lru_reader_cache &&other) = default;
    ~lru_reader_cache() = default;

    lru_reader_cache& operator=(const lru_reader_cache &other) = default;
    lru_reader_cache& operator=(lru_reader_cache &&other) = default;

    std::shared_ptr<const reader> get_reader(const std::string &path);

private:
    using reader_context = std::pair<std::string, std::shared_ptr<reader>>;

    std::reference_wrapper<const reader_manager> m_reader_manager;
    std::size_t m_max_open;
    std::list<reader_context> m_open_readers;
    std::unordered_map<std::string, std::list<reader_context>::const_iterator> m_reader_paths;

};

} // namespace image
} // namespace xmipp4
