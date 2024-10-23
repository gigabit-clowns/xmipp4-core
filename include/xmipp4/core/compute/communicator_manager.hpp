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
 * @file communicator_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines communicator_manager interface
 * @date 2024-04-29
 * 
 */

#include <string>

namespace xmipp4 
{
namespace compute
{

class communicator;

class communicator_manager
{
public:
    communicator_manager() = default;
    communicator_manager(const communicator_manager &other) = default;
    communicator_manager(communicator_manager &&other) = default;
    virtual ~communicator_manager() = default;

    communicator_manager& operator=(const communicator_manager &other) = default;
    communicator_manager& operator=(communicator_manager &&other) = default;

    virtual const std::string& get_name() const noexcept = 0;

    virtual communicator& get_world_communicator() const = 0;

}; 

} // namespace compute
} // namespace xmipp4
