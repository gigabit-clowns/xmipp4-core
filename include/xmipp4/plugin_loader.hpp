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
 * @file plugin_loader.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines plugin_loader class
 * @date 2024-03-11
 * 
 */

#include "memory/pimpl.hpp"
#include "platform/dynamic_shared_object.h"

namespace xmipp4
{

class plugin;

class plugin_loader
{
public:
    XMIPP4_CORE_API plugin_loader();
    XMIPP4_CORE_API plugin_loader(const std::string& name);
    plugin_loader(const plugin_loader& other) = delete;
    XMIPP4_CORE_API plugin_loader(plugin_loader&& other);
    XMIPP4_CORE_API ~plugin_loader();

    plugin_loader& operator=(const plugin_loader& other) = delete;
    XMIPP4_CORE_API plugin_loader& operator=(plugin_loader&& other);

    XMIPP4_CORE_API const plugin* begin() const noexcept;
    XMIPP4_CORE_API const plugin* end() const noexcept;

    XMIPP4_CORE_API std::size_t count() const noexcept;

    XMIPP4_CORE_API bool is_open() const noexcept;
    XMIPP4_CORE_API void reset();

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

};

}
