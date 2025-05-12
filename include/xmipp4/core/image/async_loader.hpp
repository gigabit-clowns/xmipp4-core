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
 * @file async_loader.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::async_loader class
 * @date 2025-05-07
 * 
 */

#include "async_load_result.hpp"
#include "location.hpp"
#include "../span.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace image
{

class reader_manager;

/**
 * @brief Base class to efficiently read images asynchronously.
 * 
 * This base class may be implemented to read images from potentially 
 * multiple image stacks. Implementations may choose different policies to 
 * maximize the throughput of the reading process. 
 * 
 */
class async_loader
{
public:
    async_loader() = default;
    async_loader(const async_loader &other) = default;
    async_loader(async_loader &&other) = default;
    virtual ~async_loader() = default;

    async_loader &operator=(const async_loader &other) = default;
    async_loader &operator=(async_loader &&other) = default;
    
    /**
     * @brief Read a batch of images.
     * 
     * @param locations Locations of the images to read.
     * @return async_load_result Result object to be polled.
     */
    virtual std::unique_ptr<async_load_result> 
    read_batch(span<const location> locations) = 0;

    /**
     * @brief Read a single image.
     * 
     * @param location Location where the image is stored.
     * @return async_load_result Result object to be polled.
     */
    virtual std::unique_ptr<async_load_result> 
    read_single(const location &location) = 0;
    
};

} // namespace image
} // namespace xmipp4
