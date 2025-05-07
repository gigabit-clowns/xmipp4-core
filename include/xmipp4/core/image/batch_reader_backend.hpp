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
 * @file batch_reader_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::batch_reader_backend class
 * @date 2025-05-07
 * 
 */

#include "../backend.hpp"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace image
{

class batch_reader;
class reader_manager;

/**
 * @brief Abstract factory class to create batch_reader-s. 
 * 
 */
class batch_reader_backend
    : public backend
{
public:
    batch_reader_backend() = default;
    batch_reader_backend(const batch_reader_backend &) = default;
    batch_reader_backend(batch_reader_backend &&) = default;
    virtual ~batch_reader_backend() = default;
    
    batch_reader_backend &operator=(const batch_reader_backend &) = default;
    batch_reader_backend &operator=(batch_reader_backend &&) = default;

    virtual std::shared_ptr<batch_reader> 
    create_batch_reader(reader_manager &readers) const = 0;

};

} // namespace image
} // namespace xmipp4
