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
 * @file communicator_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines communicator_backend interface
 * @date 2024-10-24
 * 
 */

#include "../backend.hpp"

#include "../platform/dynamic_shared_object.h"

#include <string>
#include <memory>

namespace xmipp4 
{
namespace communication
{

class communicator;

class XMIPP4_CORE_API communicator_backend
    : public backend
{
public:
    communicator_backend() = default;
    communicator_backend(const communicator_backend &other) = default;
    communicator_backend(communicator_backend &&other) = default;
    virtual ~communicator_backend() = default;

    communicator_backend& operator=(const communicator_backend &other) = default;
    communicator_backend& operator=(communicator_backend &&other) = default;

    /**
     * @brief Get the world communicator.
     * 
     * The world communicator connects all known peers together.
     * 
     * @return std::shared_ptr<communicator> Reference to the world
     * communicator.
     */
    virtual std::shared_ptr<communicator> create_world_communicator() const = 0;

}; 

} // namespace communication
} // namespace xmipp4
