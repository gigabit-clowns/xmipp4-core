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
 * @file lru_batch_loader.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::lru_batch_loader class
 * @date 2025-05-07
 * 
 */

#include <xmipp4/core/image/batch_loader.hpp>

#include "lru_reader_cache.hpp"

namespace xmipp4 
{
namespace image
{

/**
 * @brief Specialized batch_loader that keeps several files open on a Least
 * Recently Used (LRU) polcy basis.
 * policy.
 * 
 */
class lru_batch_loader final
    : public batch_loader
{
public: 
    /**
     * @brief Construct a new lru_batch_loader.
     * 
     * @param readers reader_manager from which necessary readers are created.
     * @param max_open Maximum amount of readers open at a given time.
     * When set to 0, no readers are cached.
     */
    explicit lru_batch_loader(const reader_manager &readers, 
                                  std::size_t max_open = 64 );
    lru_batch_loader(const lru_batch_loader &other) = delete;
    lru_batch_loader(lru_batch_loader &&other) = default;
    ~lru_batch_loader() override = default;

    lru_batch_loader &operator=(const lru_batch_loader &other) = delete;
    lru_batch_loader &operator=(lru_batch_loader &&other) = default;
    
    void read_batch(span<const location> locations) override; // TODO return

private:
    lru_reader_cache m_cache;

};

} // namespace image
} // namespace xmipp4
