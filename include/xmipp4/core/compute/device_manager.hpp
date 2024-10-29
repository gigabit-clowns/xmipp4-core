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
 * @file device_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines device_manager interface
 * @date 2024-10-23
 * 
 */

#include "device_index.hpp"
#include "device_properties.hpp"
#include "../interface_manager.hpp"
#include "../memory/pimpl.hpp"

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace compute
{

class device;
class device_backend;

class device_manager
    : public interface_manager
{
public:
    device_manager();
    device_manager(const device_manager &other) = delete;
    device_manager(device_manager &&other);
    virtual ~device_manager();

    device_manager& operator=(const device_manager &other) = delete;
    device_manager& operator=(device_manager &&other);

    bool register_backend(std::unique_ptr<device_backend> backend);

    void enumerate_backends(std::vector<std::string> &backends) const;
    device_backend* get_backend(const std::string &name) const;

    void enumerate_devices(std::vector<device_index> &indices) const;

    bool get_device_properties(const device_index &index, 
                               device_properties &desc ) const;

    std::unique_ptr<device> 
    create_device(const device_index &index) const;
    std::shared_ptr<device> 
    create_device_shared(const device_index &index) const;

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;


}; 

} // namespace compute
} // namespace xmipp4
