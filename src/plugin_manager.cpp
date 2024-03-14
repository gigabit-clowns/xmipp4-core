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
 * @file plugin_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of plugin.hpp
 * @date 2024-03-11
 * 
 */

#include <xmipp4/plugin_manager.hpp>

namespace xmipp4
{

class plugin_manager::implementation
{
public:

private:

};

plugin_manager::plugin_manager() = default;

plugin_manager::plugin_manager(plugin_manager&& other) = default;

plugin_manager::~plugin_manager() = default;

plugin_manager& plugin_manager::operator=(plugin_manager&& other) = default;

} // namespace xmipp4
