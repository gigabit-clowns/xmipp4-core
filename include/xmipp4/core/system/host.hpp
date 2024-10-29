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
 * @file host.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Utilities to query the host
 * @date 2024-10-29
 * 
 */

#include <string>

namespace xmipp4 
{
namespace system
{

/**
 * @brief Get the hostname of the machine.
 * 
 * @return std::string The hostname.
 */
std::string get_hostname();

/**
 * @brief Get the total memory available in the system.
 * 
 * @return std::size_t Total memory in bytes.
 */
std::size_t get_total_system_memory();

} // namespace system
} // namespace xmipp4
