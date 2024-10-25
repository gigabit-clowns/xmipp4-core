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
 * @file host_communicator_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines host_communicator_manager interface
 * @date 2024-10-25
 * 
 */

#include <memory>
#include <unordered_map>

#include "../interface_manager.hpp"

namespace xmipp4 
{
namespace compute
{

class host_communicator;
class host_communicator_backend;

/**
 * @brief Centralizes all known implementations of the host_communicator_backend
 * interface.
 * 
 */
class host_communicator_manager
    : public interface_manager
{
public:
    host_communicator_manager() = default;
    host_communicator_manager(const host_communicator_manager &other) = default;
    host_communicator_manager(host_communicator_manager &&other) = default;
    virtual ~host_communicator_manager() = default;

    host_communicator_manager& operator=(const host_communicator_manager &other) = default;
    host_communicator_manager& operator=(host_communicator_manager &&other) = default;

    /**
     * @brief Register a new implementation.
     * 
     * @param backend The backend to be registered.
     * @return true Successfully registered.
     * @return false Failed to register. I.e. an homonym implementation 
     * already exists or nullptr was provided.
     */
    bool register_backend(std::unique_ptr<host_communicator_backend> backend);

    /**
     * @brief Find a backend by its name.
     * 
     * @param name The name of the backend.
     * @return host_communicator_backend* The backend. Nullptr if not found.
     */
    host_communicator_backend* 
    get_backend(const std::string &name) const;

    /**
     * @brief Get the world communicator of a backend.
     * 
     * The world communicator connects all known peers together.
     * 
     * @param name The name of the backend.
     * @return std::shared_ptr<host_communicator> Reference to the world
     * communicator.
     */
    std::shared_ptr<host_communicator> 
    get_world_communicator(const std::string &name) const;

private:
    using registry_type = 
        std::unordered_map<std::string, std::unique_ptr<host_communicator_backend>>;

    registry_type m_registry;

}; 

} // namespace compute
} // namespace xmipp4
