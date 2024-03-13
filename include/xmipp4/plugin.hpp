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
 * @file plugin.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines plugin class
 * @date 2024-03-11
 * 
 */

#include "version.hpp"
#include "memory/pimpl.hpp"
#include "platform/dynamic_shared_object.h"

#include <string>

namespace xmipp4
{

class plugin
{
public:
    XMIPP4_CORE_API plugin(const std::string& name, version version);
    XMIPP4_CORE_API plugin(const plugin& other);
    XMIPP4_CORE_API plugin(plugin&& other);
    XMIPP4_CORE_API ~plugin();

    XMIPP4_CORE_API plugin& operator=(const plugin& other);
    XMIPP4_CORE_API plugin& operator=(plugin&& other);

    XMIPP4_CORE_API void set_name(const std::string& name);
    XMIPP4_CORE_API const std::string& get_name() const noexcept;

    XMIPP4_CORE_API void set_version(version version) noexcept;
    XMIPP4_CORE_API version get_version() const noexcept;

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

};

}
