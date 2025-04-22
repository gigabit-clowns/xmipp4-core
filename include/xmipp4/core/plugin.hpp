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
#include "platform/dynamic_shared_object.h"

#include <string>

namespace xmipp4
{

class interface_catalog;

/**
 * @brief Abstract class representing a plugin interface.
 * 
 * A concrete instance of this class may be used to connect a plugin
 * to the core.
 * 
 */
class XMIPP4_CORE_API plugin
{
public:
    plugin() = default;
    plugin(const plugin& other) = default;
    plugin(plugin&& other) = default;
    virtual ~plugin() = default;

    plugin& operator=(const plugin& other) = default;
    plugin& operator=(plugin&& other) = default;

    /**
     * @brief Get the name of the plugin.
     * 
     * @return const std::string& The name.
     */
    virtual const std::string& get_name() const noexcept = 0;

    /**
     * @brief Get the version of the plugin.
     * 
     * @return version The version.
     */
    virtual version get_version() const noexcept = 0;

    /**
     * @brief Register this plugin at a given interface catalog.
     * 
     * @param catalog The catalog where this plugin will be registered.
     */
    virtual void register_at(interface_catalog& catalog) const = 0;

};

} // namespace xmipp4
