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
 * @file host_unified_buffer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::host_unified_buffer interface
 * @date 2024-10-29
 * 
 */

#include "../device_buffer.hpp"
#include "../host_buffer.hpp"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Interface merging the interfaces of host_buffer and device_buffer.
 * 
 * This interface unifies host_buffer and device_buffer interfaces, as
 * when using the host as a compute device, both buffers types are 
 * equivalent
 * 
 */
class host_unified_buffer
    : public device_buffer
    , public host_buffer
{
public:
    host_unified_buffer() = default;
    host_unified_buffer(const host_unified_buffer &other) = default;
    host_unified_buffer(host_unified_buffer &&other) = default;
    ~host_unified_buffer() override = default;

    host_unified_buffer& operator=(const host_unified_buffer &other) = default;
    host_unified_buffer& operator=(host_unified_buffer &&other) = default;

    numerical_type get_type() const noexcept override = 0;
    std::size_t get_count() const noexcept override = 0;
    void* get_data() noexcept override = 0;
    const void* get_data() const noexcept override = 0;

}; 

} // namespace compute
} // namespace xmipp4
