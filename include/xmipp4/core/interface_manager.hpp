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
 * @file interface_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines interface_manager class
 * @date 2024-10-23
 * 
 */

#include "platform/dynamic_shared_object.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace xmipp4
{

/**
 * @brief Abstract class defining the interface of an interface manager.
 * 
 * An interface manager keeps track of all known implementations of a given
 * interface and provides utilities to access the most relevant implementation
 * for a given context. This is a base class for collecting them on an
 * interface_catalog.
 * 
 * @see interface_catalog
 * 
 */
class XMIPP4_CORE_API interface_manager
{
public:
    interface_manager() = default;
    interface_manager(const interface_manager& other) = default;
    interface_manager(interface_manager&& other) = default;
    virtual ~interface_manager() = default;

    interface_manager& operator=(const interface_manager& other) = default;
    interface_manager& operator=(interface_manager&& other) = default;

    /**
     * @brief Register backends bundled with the core.access_flag_bits
     * 
     * This usually loads fallaback implementations that are always 
     * available although they may not be the most efficient or performant.
     * The loaded backend(s) depend on the implementation of the specific
     * interface.
     * 
     */
    virtual void register_builtin_backends() = 0;

};

template <typename B>
class basic_interface_manager
    : public interface_manager
{
public:
    /**
     * @brief Alias of the backend template parameter provided
     * when instantiating this class.
     * 
     */
    using backend_type = B;

    basic_interface_manager() = default;
    basic_interface_manager(const basic_interface_manager &other) = delete;
    basic_interface_manager(basic_interface_manager &&other) = default;
    virtual ~basic_interface_manager() override = default;

    basic_interface_manager& operator=(const basic_interface_manager &other) = delete;
    basic_interface_manager& operator=(basic_interface_manager &&other) = default;

    /**
     * @brief Register a new implementation.
     * 
     * @param backend The backend to be registered.
     * @return true Successfully registered.
     * @return false Failed to register. I.e. an homonym implementation 
     * already exists or nullptr was provided.
     * 
     */
    bool register_backend(std::unique_ptr<backend_type> backend);

    /**
     * @brief Enumerate all available backends.
     * 
     * @param backends Output list with the names of the backends.
     * @note The list is cleared before appending the backend names.
     */
    void enumerate_backends(std::vector<std::string> &backends) const;

    /**
     * @brief Find a backend by its name.
     * 
     * @param name The name of the backend.
     * @return backend_type* The backend. Nullptr if not found.
     */
    backend_type* get_backend(const std::string &name) const;

    /**
     * @brief Get all the backends known by this manager.
     * 
     * @param backends Output parameter where backends are written.
     * 
     * @note Caller must not manage the lifetime of the returned objects.
     * 
     */
    void get_backends(std::vector<backend_type*> &backends) const;

    /**
     * @brief Get all the backends known by this manager.
     * 
     * @param backends Output parameter where backends are written.
     * 
     * @note Caller must not manage the lifetime of the returned objects.
     * 
     */
    void get_available_backends(std::vector<backend_type*> &backends) const;

private:
    std::unordered_map<std::string, std::unique_ptr<backend_type>> m_backends;

};

} // namespace xmipp4

#include "interface_manager.inl"
