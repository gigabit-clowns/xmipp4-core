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
 * @file caching_batch_reader.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::caching_batch_reader class
 * @date 2025-05-07
 * 
 */

#include "batch_reader.hpp"

#include "../memory/pimpl.hpp"

namespace xmipp4 
{
namespace image
{

/**
 * @brief Specialized batch_reader that keeps a cache of opened image files.
 * 
 */
class caching_batch_reader final
    : public batch_reader
{
public:
    explicit caching_batch_reader(const reader_manager &readers, 
                                  std::size_t max_open = 64 );
    caching_batch_reader(const caching_batch_reader &) = delete;
    caching_batch_reader(caching_batch_reader &&);
    ~caching_batch_reader();

    caching_batch_reader &operator=(const caching_batch_reader &) = delete;
    caching_batch_reader &operator=(caching_batch_reader &&);
    
    void read_batch(span<const location> locations) override; // TODO return

private:
    class implementation;
    memory::pimpl<implementation> m_impl;

};

} // namespace image
} // namespace xmipp4
