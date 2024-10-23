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
 * @file memory_mapped_file.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of memory_mapped_file.hpp
 * @date 2023-08-13
 * 
 */

#include <xmipp4/core/system/memory_mapped_file.hpp>

#include "memory_mapped_file_handle.hpp"

namespace xmipp4
{
namespace system
{

memory_mapped_file::memory_mapped_file() noexcept
    : m_size(0UL)
    , m_data(nullptr)
{
}

memory_mapped_file::memory_mapped_file(const char* filename, 
                                       access_flags access,
                                       std::size_t size,
                                       bool copy_on_write )
    : m_size(size)
    , m_data(memory_mapped_file_open(filename, access, m_size, copy_on_write))
{
}

memory_mapped_file::memory_mapped_file(const std::string& filename, 
                                       access_flags access,
                                       std::size_t size,
                                       bool copy_on_write )
    : memory_mapped_file(filename.c_str(), access, size, copy_on_write)
{
}

memory_mapped_file::memory_mapped_file(memory_mapped_file&& other) noexcept
    : m_size(other.m_size)
    , m_data(other.m_data)
{
    other.m_size = 0UL;
    other.m_data = nullptr;
}

memory_mapped_file::~memory_mapped_file()
{
    close();
}

memory_mapped_file& memory_mapped_file::operator=(memory_mapped_file&& other) noexcept
{
    close();
    swap(other);
    return *this;
}

bool memory_mapped_file::is_open() const noexcept
{
    return m_data != nullptr;
}

void memory_mapped_file::swap(memory_mapped_file& other) noexcept
{
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
}

void memory_mapped_file::open(const char* filename, 
                              access_flags access,
                              std::size_t size,
                              bool copy_on_write )
{
    *this = memory_mapped_file(filename, access, size, copy_on_write);
}

void memory_mapped_file::open(const std::string& filename, 
                              access_flags access,
                              std::size_t size,
                              bool copy_on_write )
{
    open(filename.c_str(), access, size, copy_on_write);
}

void memory_mapped_file::close() noexcept
{
    if (m_data != nullptr)
    {
        memory_mapped_file_close(m_data, m_size);
        m_size = 0UL;
        m_data = nullptr;
    }
}

std::size_t memory_mapped_file::size() const noexcept
{
    return m_size;
}

void* memory_mapped_file::data() const noexcept
{
    return m_data;
}



void swap(memory_mapped_file& lhs, memory_mapped_file& rhs) noexcept
{
    lhs.swap(rhs);
}

} // namespace system
} // namespace xmipp4
