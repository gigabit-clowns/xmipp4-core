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
 * @file dynamic_library.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of dynamic_library.hpp
 * @date 2023-08-13
 * 
 */

#include <xmipp4/system/dynamic_library.hpp>

#include "dynamic_library_handle.hpp"

namespace xmipp4
{
namespace system
{

dynamic_library::dynamic_library() noexcept
    : m_handle(nullptr)
{
}

dynamic_library::dynamic_library(const char* filename)
    : m_handle(dynamic_library_open(filename))
{
}

dynamic_library::dynamic_library(const std::string& filename)
    : dynamic_library(filename.c_str())
{
}

dynamic_library::dynamic_library(dynamic_library&& other) noexcept
    : m_handle(other.m_handle)
{
    other.m_handle = nullptr;
}

dynamic_library::~dynamic_library()
{
    close();
}

dynamic_library& dynamic_library::operator=(dynamic_library&& other) noexcept
{
    close();
    swap(other);
    return *this;
}

dynamic_library::operator bool() const noexcept
{
    return m_handle != nullptr;
}



void dynamic_library::swap(dynamic_library& other) noexcept
{
    std::swap(m_handle, other.m_handle);
}

void dynamic_library::open(const char* filename)
{
    *this = dynamic_library(filename);
}

void dynamic_library::open(const std::string& filename)
{
    *this = dynamic_library(filename);
}

void dynamic_library::close() noexcept
{
    if (m_handle)
    {
        dynamic_library_close(m_handle);
        m_handle = nullptr;
    }
}

void* dynamic_library::get_symbol(const char* name) const noexcept
{
    return dynamic_library_get_symbol(m_handle, name);
}

void* dynamic_library::get_symbol(const std::string& name) const noexcept
{
    return get_symbol(name.c_str());
}

void swap(dynamic_library& lhs, dynamic_library& rhs) noexcept
{
    lhs.swap(rhs);
}

} // namespace system
} // namespace xmipp4
