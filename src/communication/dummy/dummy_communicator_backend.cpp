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
 * @file dummy_communicator_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of dummy_communicator_backend.hpp
 * @date 2024-10-26
 * 
 */

#include <xmipp4/core/communication/dummy/dummy_communicator_backend.hpp>

#include <xmipp4/core/communication/dummy/dummy_communicator.hpp>
#include <xmipp4/core/communication/communicator_manager.hpp>

#include <xmipp4/core/core_version.hpp>

namespace xmipp4 
{
namespace communication
{

std::string dummy_communicator_backend::get_name() const noexcept
{
    return "dummy";
}

version dummy_communicator_backend::get_version() const noexcept
{
    return get_core_version();
}

bool dummy_communicator_backend::is_available() const noexcept
{
    return true;
}

backend_priority dummy_communicator_backend::get_priority() const noexcept
{
    return backend_priority::fallback;
}

std::shared_ptr<communicator> 
dummy_communicator_backend::get_world_communicator() const
{
    return std::make_shared<dummy_communicator>();
}

bool dummy_communicator_backend::register_at(communicator_manager &manager)
{
    return manager.register_backend(
        std::make_unique<dummy_communicator_backend>()
    );
}

} // namespace communication
} // namespace xmipp4
