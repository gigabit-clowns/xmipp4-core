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
 * @file communicator_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines communicator_manager interface
 * @date 2024-10-25
 * 
 */

#include <string>
#include <vector>
#include <memory>

#include "communicator_backend.hpp"
#include "../interface_manager.hpp"
#include "../platform/dynamic_shared_object.h"
#include "../memory/pimpl.hpp"

namespace xmipp4 
{
namespace communication
{

class communicator;

/**
 * @brief Centralizes all known implementations of the communicator_backend
 * interface.
 * 
 */
class communicator_manager final
    : public interface_manager
{
public:
    XMIPP4_CORE_API communicator_manager();
    communicator_manager(const communicator_manager &other) = delete;
    XMIPP4_CORE_API 
    communicator_manager(communicator_manager &&other) noexcept;
    XMIPP4_CORE_API virtual ~communicator_manager();

    communicator_manager& 
    operator=(const communicator_manager &other) = delete;
    XMIPP4_CORE_API communicator_manager& 
    operator=(communicator_manager &&other) noexcept;

    XMIPP4_CORE_API
    void load_builtin_backends() override;

    /**
     * @brief Register a new implementation.
     * 
     * @param backend The backend to be registered.
     * @return true Successfully registered.
     * @return false Failed to register. I.e. an homonym implementation 
     * already exists or nullptr was provided.
     */
    XMIPP4_CORE_API 
    bool register_backend(std::unique_ptr<communicator_backend> backend);

    /**
     * @brief Enumerate all available backends.
     * 
     * @param backends Output list with the names of the backends.
     * @note The list is cleared before appending the backend names.
     */
    XMIPP4_CORE_API 
    void enumerate_backends(std::vector<std::string> &backends) const;


    /**
     * @brief Find a backend by its name.
     * 
     * @param name The name of the backend.
     * @return communicator_backend* The backend. Nullptr if not found.
     */
    XMIPP4_CORE_API
    communicator_backend* 
    get_backend(const std::string &name) const;
    
    /**
     * @brief Find the most suitable backend
     *
     * The most suitable backend is an available backend with the highest
     * priority.
     *  
     * @return communicator_backend* The backend. Nullptr if none is available.
     */
    XMIPP4_CORE_API
    communicator_backend* get_preferred_backend() const;

    /**
     * @brief Get the world communicator of a backend.
     * 
     * The world communicator connects all known peers together.
     * 
     * @param name The name of the backend.
     * @return std::shared_ptr<communicator> Reference to the world
     * communicator.
     */
    XMIPP4_CORE_API
    std::shared_ptr<communicator> 
    create_world_communicator(const std::string &name) const;

    /**
     * @brief Get the world communicator from the preferred backend.
     * 
     * @return std::shared_ptr<communicator> Reference to the world
     * communicator.
     * @see get_preferred_backend
     */
    XMIPP4_CORE_API
    std::shared_ptr<communicator> create_preferred_world_communicator() const;

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

}; 

} // namespace communication
} // namespace xmipp4
