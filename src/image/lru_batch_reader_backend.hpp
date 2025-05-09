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
 * @file lru_batch_reader_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::lru_batch_reader_backend class
 * @date 2025-05-07
 * 
 */

#include <xmipp4/core/image/batch_reader_backend.hpp>

namespace xmipp4 
{
namespace image
{

class batch_reader_manager;

/**
 * @brief Backend to create a lru_batch_reader-s.
 * 
 * @see lru_batch_reader
 *  
 */
class lru_batch_reader_backend
    : public batch_reader_backend
{
public:
    lru_batch_reader_backend() = default;
    lru_batch_reader_backend(const lru_batch_reader_backend &other) = default;
    lru_batch_reader_backend(lru_batch_reader_backend &&other) = default;
    virtual ~lru_batch_reader_backend() = default;
    
    lru_batch_reader_backend &operator=(const lru_batch_reader_backend &other) = default;
    lru_batch_reader_backend &operator=(lru_batch_reader_backend &&other) = default;

    std::string get_name() const noexcept override;
    version get_version() const noexcept override;
    bool is_available() const noexcept override;
    backend_priority get_priority() const noexcept override;
    std::shared_ptr<batch_reader> 
    create_batch_reader(const reader_manager &reader_manager) const override;

    static bool register_at(batch_reader_manager &manager);

};

} // namespace image
} // namespace xmipp4
