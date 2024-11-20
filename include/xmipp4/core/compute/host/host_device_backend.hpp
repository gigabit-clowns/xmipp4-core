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
 * @file host_device_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines host_device_backend interface
 * @date 2024-10-29
 * 
 */

#include "../device_backend.hpp"

namespace xmipp4 
{
namespace compute
{

class device_manager;

/**
 * @brief Special implementation of the device_backend interface to be able 
 * to obtain the "host" as a device..
 * 
 */
class host_device_backend final
    : public device_backend
{
public:
    std::string get_name() const noexcept override;
    version get_version() const noexcept override;
    bool is_available() const noexcept override;
    backend_priority get_priority() const noexcept override;

    void enumerate_devices(std::vector<std::size_t> &ids) const override;
    bool get_device_properties(std::size_t id, 
                               device_properties &desc) const override;

    std::unique_ptr<device> create_device(std::size_t id) override;
    std::shared_ptr<device> create_device_shared(std::size_t id) override;

    static bool register_at(device_manager &manager);

}; 

} // namespace compute
} // namespace xmipp4
