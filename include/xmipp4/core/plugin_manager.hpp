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
 * @file plugin_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines plugin_manager class
 * @date 2024-03-11
 * 
 */

#include "memory/pimpl.hpp"
#include "platform/dynamic_shared_object.h"

namespace xmipp4
{

class plugin;

class XMIPP4_CORE_API plugin_manager
{
public:
    plugin_manager();
    plugin_manager(const std::string& name);
    plugin_manager(const plugin_manager& other) = delete;
    plugin_manager(plugin_manager&& other);
    ~plugin_manager();

    plugin_manager& operator=(const plugin_manager& other) = delete;
    plugin_manager& operator=(plugin_manager&& other);

    void add_plugin(const plugin& plugin);
    const plugin* load_plugin(const std::string &path);

    std::size_t count() const noexcept;
    const plugin& get_plugin(std::size_t index) const;

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

};

} // namespace xmipp4
