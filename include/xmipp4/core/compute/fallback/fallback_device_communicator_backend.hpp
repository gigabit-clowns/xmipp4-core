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
 * @file fallback_device_communicator_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines fallback_device_communicator_backend interface
 * @date 2024-11-25
 * 
 */

#include "../device_communicator_backend.hpp"

namespace xmipp4 
{
namespace compute
{

class fallback_device_communicator_backend final
    : public device_communicator_backend
{
public:
    std::string get_name() const noexcept override;
    version get_version() const noexcept override;
    bool is_available() const noexcept override;
    backend_priority get_priority() const noexcept override;

    bool supports_devices(span<device*> devices) const noexcept override;

    void create_device_communicators(node_communicator &node_communicator,
                                     span<device*> devices,
                                     span<std::unique_ptr<device_communicator>> result ) const override;
    
    void create_device_communicators_shared(node_communicator &node_communicator,
                                            span<device*> devices,
                                            span<std::shared_ptr<device_communicator>> result ) const override;

}; 

} // namespace compute
} // namespace xmipp4
