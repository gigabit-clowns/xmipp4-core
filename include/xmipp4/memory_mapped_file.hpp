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

#include "platform/constexpr.hpp"
#include "access_flags.hpp"
#include "utils/byte.hpp"

#include <type_traits>

namespace xmipp4 {

class memory_mapped_file
{
public:
    static inline XMIPP4_CONST_CONSTEXPR std::size_t whole_file = 0UL;

    memory_mapped_file() noexcept;
    explicit memory_mapped_file(const char* filename, 
                                std::ptrdiff_t offset = 0,
                                std::size_t size = whole_file,
                                access_flags access = read_only );
    explicit memory_mapped_file(const std::string& filename, 
                                std::ptrdiff_t offset = 0,
                                std::size_t size = whole_file,
                                access_flags access = read_only );
    memory_mapped_file(const memory_mapped_file& other) = delete;
    memory_mapped_file(memory_mapped_file&& other) noexcept;
    ~memory_mapped_file();

    memory_mapped_file& operator=(const memory_mapped_file& other) = delete;
    memory_mapped_file& operator=(memory_mapped_file&& other) noexcept;

    operator bool() const noexcept;

    void swap(memory_mapped_file& other) noexcept;

    void open(const char* filename, 
              std::ptrdiff_t offset = 0,
              std::size_t size = whole_file,
              access_flags access = read_only );
    void open(const std::string& filename, 
              std::ptrdiff_t offset = 0,
              std::size_t size = whole_file,
              access_flags access = read_only );
    void close() noexcept;

    std::size_t size() const noexcept;
    void* data() const noexcept;

private:
    std::size_t m_size;
    void* m_data;

};

void swap(memory_mapped_file& lhs, memory_mapped_file& rhs) noexcept;

} //namespace xmipp4
