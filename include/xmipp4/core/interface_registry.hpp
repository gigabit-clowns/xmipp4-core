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

#include <unordered_map>
#include <typeindex>
#include <memory>

namespace xmipp4
{

class interface_manager;

class interface_registry
{
public:
    interface_registry() = default;
    interface_registry(const interface_registry& other) = default;
    interface_registry(interface_registry&& other) = default;
    ~interface_registry() = default;

    interface_registry& operator=(const interface_registry& other) = default;
    interface_registry& operator=(interface_registry&& other) = default;

    template <typename T>
    typename std::enable_if<std::is_convertible<T*, interface_manager*>::value, T&>::type
    get_interface();

private:
    std::unordered_map<std::type_index, std::unique_ptr<interface_manager>> m_interfaces;

};

} // namespace xmipp4

#include "interface_registry.inl"
