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
 * @file default_host_unified_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of default_host_unified_buffer.hpp
 * @date 2024-10-29
 * 
 */

#include "default_host_unified_buffer.hpp"

#include <stdexcept>
#include <cstdlib>

namespace xmipp4
{
namespace compute
{

default_host_unified_buffer::default_host_unified_buffer() noexcept
    : m_size(0)
    , m_data(nullptr)
{
}

default_host_unified_buffer
::default_host_unified_buffer(std::size_t size, std::size_t alignment)
    : m_size(size)
    , m_data(std::aligned_alloc(alignment, size))
{
    if(m_data == nullptr)
    {
        throw std::bad_alloc();
    }
}

default_host_unified_buffer
::default_host_unified_buffer(default_host_unified_buffer &&other) noexcept
    : m_size(other.m_size)
    , m_data(nullptr)
{
}

default_host_unified_buffer::~default_host_unified_buffer()
{
    reset();
}

default_host_unified_buffer& 
default_host_unified_buffer
::operator=(default_host_unified_buffer &&other) noexcept
{
    swap(other);
    other.reset();
    return *this;
}

void default_host_unified_buffer
::swap(default_host_unified_buffer &other) noexcept
{
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
}

void default_host_unified_buffer::reset() noexcept
{
    if (m_data)
    {
        free(m_data);
        m_size = 0;
        m_data = nullptr;
    }
}

std::size_t default_host_unified_buffer::get_size() const noexcept
{
    return m_size;
}

void* default_host_unified_buffer::get_data() noexcept
{
    return m_data;
}

const void* default_host_unified_buffer::get_data() const noexcept
{
    return m_data;
}

void default_host_unified_buffer::record_queue(device_queue&)
{
    // No-op
}

} // namespace compute
} // namespace xmipp4
