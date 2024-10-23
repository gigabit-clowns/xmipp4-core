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

#include <string>

namespace xmipp4
{

class interface_manager;

class plugin
{
public:
    plugin() = default;
    plugin(const plugin& other) = default;
    plugin(plugin&& other) = default;
    virtual ~plugin() = default;

    plugin& operator=(const plugin& other) = default;
    plugin& operator=(plugin&& other) = default;

    virtual const std::string& get_name() const noexcept = 0;
    virtual version get_version() const noexcept = 0;
    virtual void register_at(interface_manager& manager) const = 0;
    virtual void deregister_at(interface_manager& manager) const = 0;

};

}