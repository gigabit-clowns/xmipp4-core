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

#include <unordered_map>
#include <typeindex>
#include <memory>

namespace xmipp4
{

class managed_interface;

class interface_manager
{
public:
    interface_manager() = default;
    interface_manager(const interface_manager& other) = default;
    interface_manager(interface_manager&& other) = default;
    ~interface_manager() = default;

    interface_manager& operator=(const interface_manager& other) = default;
    interface_manager& operator=(interface_manager&& other) = default;

    template <typename T>
    typename std::enable_if<std::is_convertible<T*, managed_interface*>::value, T&>::type
    get_interface();

private:
    std::unordered_map<std::type_index, std::unique_ptr<managed_interface>> m_interfaces;

};

} // namespace xmipp4
