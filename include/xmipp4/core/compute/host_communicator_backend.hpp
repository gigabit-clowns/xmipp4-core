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
 * @file host_communicator_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines host_communicator_backend interface
 * @date 2024-10-24
 * 
 */

#include "../version.hpp"

#include "../platform/dynamic_shared_object.h"

#include <string>
#include <memory>

namespace xmipp4 
{
namespace compute
{

class host_communicator;

class XMIPP4_CORE_API host_communicator_backend
{
public:
    host_communicator_backend() = default;
    host_communicator_backend(const host_communicator_backend &other) = default;
    host_communicator_backend(host_communicator_backend &&other) = default;
    virtual ~host_communicator_backend() = default;

    host_communicator_backend& operator=(const host_communicator_backend &other) = default;
    host_communicator_backend& operator=(host_communicator_backend &&other) = default;

    /**
     * @brief Get the name of the implementation.
     * 
     * @return const std::string& The name.
     */
    virtual const std::string& get_name() const noexcept = 0;

    /**
     * @brief Get the version of the backend.
     * 
     * @return version The version of the implementation.
     */
    virtual version get_version() const noexcept = 0;

    /**
     * @brief Check if the backend is usable.
     * 
     * @return true The backend is usable.
     * @return false Backend initialization has failed and it can not be used.
     */
    virtual bool is_available() const noexcept = 0;

    /**
     * @brief Get the world communicator.
     * 
     * The world communicator connects all known peers together.
     * 
     * @return std::shared_ptr<host_communicator> Reference to the world
     * communicator.
     */
    virtual std::shared_ptr<host_communicator> get_world_communicator() const = 0;

}; 

} // namespace compute
} // namespace xmipp4
