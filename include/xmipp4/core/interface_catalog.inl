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

#include "interface_catalog.hpp"

#include "platform/constexpr.hpp"
#include "platform/assert.hpp"

namespace xmipp4 
{

template <typename T>
inline
typename std::enable_if<std::is_convertible<T*, backend_manager*>::value, T&>::type
interface_catalog::get_backend_manager()
{
    const std::type_index type(typeid(T));

    T* result = static_cast<T*>(get_backend_manager(type));
    if(result == nullptr)
    {
        // Interface does not exist. Create it
        auto new_interface = std::make_unique<T>();
        result = new_interface.get();
        create_backend_manager(type, std::move(new_interface));
    }

    return *result;
}

} // namespace xmipp4
