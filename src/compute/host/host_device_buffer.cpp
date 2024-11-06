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
 * @file host_device_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_device_buffer.hpp
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/compute/host/host_device_buffer.hpp>

#include <stdexcept>
#include <cstdlib>

namespace xmipp4
{
namespace compute
{

host_device_buffer::host_device_buffer() noexcept
    : m_type(numerical_type::unknown)
    , m_count(0)
    , m_data(nullptr)
{
}

host_device_buffer::host_device_buffer(numerical_type type, std::size_t count)
    : m_type(type)
    , m_count(count)
    , m_data(std::malloc(get_size(type)*count))
{
    if(m_data == nullptr)
    {
        throw std::bad_alloc();
    }
}

host_device_buffer::host_device_buffer(host_device_buffer &&other) noexcept
    : m_type(other.m_type)
    , m_count(other.m_count)
    , m_data(nullptr)
{
}

host_device_buffer::~host_device_buffer()
{
    reset();
}

host_device_buffer& 
host_device_buffer::operator=(host_device_buffer &&other) noexcept
{
    swap(other);
    other.reset();
    return *this;
}

void host_device_buffer::swap(host_device_buffer &other) noexcept
{
    std::swap(m_type, other.m_type);
    std::swap(m_count, other.m_count);
    std::swap(m_data, other.m_data);
}

void host_device_buffer::reset() noexcept
{
    if (m_data)
    {
        free(m_data);
        m_type = numerical_type::unknown;
        m_count = 0;
        m_data = nullptr;
    }
}

numerical_type host_device_buffer::get_type() const noexcept
{
    return m_type;
}

std::size_t host_device_buffer::get_count() const noexcept
{
    return m_count;
}

void* host_device_buffer::get_data() noexcept
{
    return m_data;
}

const void* host_device_buffer::get_data() const noexcept
{
    return m_data;
}

} // namespace compute
} // namespace xmipp4
