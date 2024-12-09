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
 * @file default_host_unified_buffer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::default_host_unified_buffer interface
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/compute/host/host_unified_buffer.hpp>

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Special implementation of device_buffer that allows to 
 * allocate memory in the host as if it were a device.
 * 
 */
class default_host_unified_buffer final
    : public host_unified_buffer
{
public:
    default_host_unified_buffer() noexcept;
    default_host_unified_buffer(std::size_t size, std::size_t alignment);
    default_host_unified_buffer(const default_host_unified_buffer &other) = delete;
    default_host_unified_buffer(default_host_unified_buffer &&other) noexcept;
    ~default_host_unified_buffer() override;

    default_host_unified_buffer& 
    operator=(const default_host_unified_buffer &other) = delete;
    default_host_unified_buffer& 
    operator=(default_host_unified_buffer &&other) noexcept;

    void swap(default_host_unified_buffer &other) noexcept;
    void reset() noexcept;

    std::size_t get_size() const noexcept override;

    void* get_data() noexcept override;
    const void* get_data() const noexcept override;

    void record_queue(device_queue &queue) override;

private:
    std::size_t m_size;
    void* m_data;

}; 

} // namespace compute
} // namespace xmipp4
