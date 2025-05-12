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
 * @file default_async_loader.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::default_async_loader class
 * @date 2025-05-07
 * 
 */

#include <xmipp4/core/image/async_loader.hpp>

#include "lru_reader_cache.hpp"

namespace xmipp4 
{
namespace image
{

/**
 * @brief Specialized async_loader that keeps several files open on a Least
 * Recently Used (LRU) polcy basis.
 * policy.
 * 
 */
class default_async_loader final
    : public async_loader
{
public: 
    /**
     * @brief Construct a new default_async_loader.
     * 
     * @param readers reader_manager from which necessary readers are created.
     * @param max_open Maximum amount of readers open at a given time.
     * When set to 0, no readers are cached.
     */
    explicit default_async_loader(const reader_manager &readers, 
                                  std::size_t max_open = 64 );
    default_async_loader(const default_async_loader &other) = delete;
    default_async_loader(default_async_loader &&other) = default;
    ~default_async_loader() override = default;

    default_async_loader &operator=(const default_async_loader &other) = delete;
    default_async_loader &operator=(default_async_loader &&other) = default;
    
    std::unique_ptr<async_load_result> read_batch(span<const location> locations) override;
    std::unique_ptr<async_load_result> read_single(const location &location) override;

private:
    lru_reader_cache m_cache;

};

} // namespace image
} // namespace xmipp4
