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
 * @file interface_registry.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines interface_registry class
 * @date 2024-10-23
 * 
 */

#include "interface_manager.hpp"

#include "memory/pimpl.hpp"
#include "platform/dynamic_shared_object.h"

#include <type_traits>
#include <typeindex>
#include <memory>

namespace xmipp4
{

/**
 * @brief Stores a single instance of each type interface_manager.
 * 
 * The interface registry allows to centralize interface managers.
 * 
 * @see interface_manager
 * 
 */
class interface_registry
{
public:
    interface_registry();
    interface_registry(const interface_registry& other) = delete;
    interface_registry(interface_registry&& other);
    ~interface_registry();

    interface_registry& operator=(const interface_registry& other) = delete;
    interface_registry& operator=(interface_registry&& other);

    /**
     * @brief Get a concrete interface manager.
     * 
     * When the method is called for the first time in a given instance with
     * a given type, it constructs a new concrete interface manager of that
     * type. If called previously, it returns the same instance.
     * 
     * @tparam T interface manager. Must be child class of interface_manager.
     * @return T& the requested interface manager.
     */
    template <typename T>
    typename std::enable_if<std::is_convertible<T*, interface_manager*>::value, T&>::type
    get_interface_manager();

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

    interface_manager* get_interface_manager(std::type_index type);
    void create_interface_manager(std::type_index type,
                                  std::unique_ptr<interface_manager> manager );

};

} // namespace xmipp4

#include "interface_registry.inl"
