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
 * @file async_loader_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::async_loader_backend class
 * @date 2025-05-07
 * 
 */

#include "../backend.hpp"

#include <string>
#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace image
{

class async_loader;
class reader_manager;

/**
 * @brief Abstract backend class to create image async_loader-s. 
 * 
 */
class async_loader_backend
    : public backend
{
public:
    async_loader_backend() = default;
    async_loader_backend(const async_loader_backend &other) = default;
    async_loader_backend(async_loader_backend &&other) = default;
    virtual ~async_loader_backend() = default;
    
    async_loader_backend &operator=(const async_loader_backend &other) = default;
    async_loader_backend &operator=(async_loader_backend &&other) = default;
    
    /**
     * @brief Create an image async_loader for the provided file.
     * 
     * @param reader_manager Reader manager from which readers are created.
     * @return std::shared_ptr<async_loader> Newly created async_loader.
     * 
     */
    virtual std::shared_ptr<async_loader> 
    create_async_loader(const reader_manager &reader_manager) const = 0;

};

} // namespace image
} // namespace xmipp4
