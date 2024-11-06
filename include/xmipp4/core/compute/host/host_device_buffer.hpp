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
 * @file host_device_buffer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::host_device_buffer interface
 * @date 2024-10-29
 * 
 */

#include "../device_buffer.hpp"
#include "../host_buffer.hpp"

namespace xmipp4 
{
namespace compute
{

class host_device_buffer final
    : public device_buffer
    , public host_buffer
{
public:
    host_device_buffer() noexcept;
    host_device_buffer(numerical_type type, std::size_t count);
    host_device_buffer(const host_device_buffer &other) = delete;
    host_device_buffer(host_device_buffer &&other) noexcept;
    virtual ~host_device_buffer();

    host_device_buffer& operator=(const host_device_buffer &other) = delete;
    host_device_buffer& operator=(host_device_buffer &&other) noexcept;

    void swap(host_device_buffer &other) noexcept;
    void reset() noexcept;

    numerical_type get_type() const noexcept final;
    std::size_t get_count() const noexcept final;

    void* get_data() noexcept final;
    const void* get_data() const noexcept final;

private:
    numerical_type m_type;
    std::size_t m_count;
    void* m_data;

}; 

} // namespace compute
} // namespace xmipp4
