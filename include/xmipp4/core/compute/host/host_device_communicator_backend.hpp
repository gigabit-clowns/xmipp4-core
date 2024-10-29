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
 * @file host_device_communicator_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines host_device_communicator_backend interface
 * @date 2024-10-29
 * 
 */

#include "../device_communicator_backend.hpp"

namespace xmipp4 
{
namespace compute
{

class device_communicator_manager;

/**
 * @brief Special implementation of the device_communicator_backend interface to be able 
 * to obtain a wrapper of communications::communicator as device_communicator.
 * 
 */
class host_device_communicator_backend final
    : public device_communicator_backend
{
public:
    const std::string& get_name() const noexcept final;
    version get_version() const noexcept final;
    bool is_available() const noexcept final;

    bool supports_device(const device &dev) const noexcept final;

    std::unique_ptr<device_communicator> 
    create_communicator(device &dev,
                        const std::shared_ptr<host_communicator> &comm) const final;
    std::shared_ptr<device_communicator> 
    create_communicator_shared(device &dev,
                               const std::shared_ptr<host_communicator> &comm) const final;

    static bool register_at(device_communicator_manager &manager);

}; 

} // namespace compute
} // namespace xmipp4
