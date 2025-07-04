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
 * @file interface_catalog.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines interface_catalog class
 * @date 2024-10-23
 * 
 */

#include "backend_manager.hpp"
#include "memory/pimpl.hpp"
#include "platform/dynamic_shared_object.h"

#include <type_traits>
#include <typeindex>
#include <memory>

namespace xmipp4
{

/**
 * @brief Stores a single instance of each type backend_manager.
 * 
 * The interface catalog allows to centralize interface managers.
 * 
 * @see backend_manager
 * 
 */
class interface_catalog
{
public:
    /**
     * @brief Construct a new interface catalog object.
     * 
     * @param register_builtin_backends This parameters controls whether the
     * interface catalog should load the backends bundled with the
     * core.
     * 
     */
    XMIPP4_CORE_API 
    explicit interface_catalog(bool register_builtin_backends = true);
    interface_catalog(const interface_catalog& other) = delete;
    XMIPP4_CORE_API interface_catalog(interface_catalog&& other) noexcept;
    XMIPP4_CORE_API ~interface_catalog();

    interface_catalog& operator=(const interface_catalog& other) = delete;
    XMIPP4_CORE_API 
    interface_catalog& operator=(interface_catalog&& other) noexcept;

    /**
     * @brief Get a concrete interface manager.
     * 
     * When the method is called for the first time in a given instance with
     * a given type, it constructs a new concrete interface manager of that
     * type. If called previously, it returns the same instance.
     * 
     * @tparam T interface manager. Must be child class of backend_manager.
     * @return T& the requested interface manager.
     */
    template <typename T>
    typename std::enable_if<std::is_convertible<T*, backend_manager*>::value, T&>::type
    get_backend_manager();

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

    XMIPP4_CORE_API
    backend_manager* get_backend_manager(std::type_index type);

    XMIPP4_CORE_API
    void create_backend_manager(std::type_index type,
                                  std::unique_ptr<backend_manager> manager );

};

} // namespace xmipp4

#include "interface_catalog.inl"
