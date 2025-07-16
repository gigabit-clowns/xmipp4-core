// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file cpu_device_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines cpu_device_backend interface
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
class cpu_device_backend final
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


    std::shared_ptr<device>
    create_device(std::size_t id,
                  const device_create_parameters &params ) override;

    static bool register_at(device_manager &manager);

}; 

} // namespace compute
} // namespace xmipp4
