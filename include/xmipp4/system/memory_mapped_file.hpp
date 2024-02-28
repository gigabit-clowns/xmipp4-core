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

#include "../access_flags.hpp"
#include "../platform/dynamic_shared_object.h"
#include "../platform/constexpr.hpp"
#include "../utils/byte.hpp"

#include <type_traits>

namespace xmipp4 
{
namespace system
{

/**
 * @brief RAII style class to memory map files
 * 
 */
class memory_mapped_file
{
public:
    /**
     * @brief Constant meaning to map the whole file
     * 
     */
    static inline XMIPP4_CONST_CONSTEXPR std::size_t whole_file = 0UL;

    /**
     * @brief Construct an empty memory-mapped file
     * 
     */
    XMIPP4_CORE_API memory_mapped_file() noexcept;

    /**
     * @brief Construct with a file opened in memory
     * 
     * @param filename Path to the file to be mapped
     * @param access Access type
     * @param size Bytes to be mapped
     */
    XMIPP4_CORE_API explicit memory_mapped_file(const char* filename, 
                                                access_flags access = read_only,
                                                std::size_t size = whole_file );

    /**
     * @brief Construct with a file opened in memory
     * 
     * @param filename Path to the file to be mapped
     * @param access Access type
     * @param size Bytes to be mapped
     */
    XMIPP4_CORE_API explicit memory_mapped_file(const std::string& filename, 
                                                access_flags access = read_only,
                                                std::size_t size = whole_file );
    memory_mapped_file(const memory_mapped_file& other) = delete;
    XMIPP4_CORE_API memory_mapped_file(memory_mapped_file&& other) noexcept;
    XMIPP4_CORE_API ~memory_mapped_file();

    memory_mapped_file& operator=(const memory_mapped_file& other) = delete;
    XMIPP4_CORE_API memory_mapped_file& operator=(memory_mapped_file&& other) noexcept;

    /**
     * @brief Check if there is a file opened
     * 
     * @return bool true if there is a file opened
     */
    XMIPP4_CORE_API bool is_open() const noexcept;

    /**
     * @brief Exchange the value with another instance
     * 
     * @param other The other instance to be exchanged with
     * 
     */
    XMIPP4_CORE_API void swap(memory_mapped_file& other) noexcept;

    /**
     * @brief Opens a file in memory
     * In case another file is open, first it closes it
     * 
     * @param filename Path to the file to be mapped
     * @param access Access type
     * @param size Bytes to be mapped
     */
    XMIPP4_CORE_API void open(const char* filename, 
                              access_flags access = read_only,
                              std::size_t size = whole_file );

    /**
     * @brief Opens a file in memory
     * In case another file is open, first it closes it
     * 
     * @param filename Path to the file to be mapped
     * @param access Access type
     * @param size Bytes to be mapped
     */
    XMIPP4_CORE_API void open(const std::string& filename, 
                              access_flags access = read_only,
                              std::size_t size = whole_file );

    /**
     * @brief Close the opened file
     * Has no effect if it is not opened
     */
    XMIPP4_CORE_API void close() noexcept;

    /**
     * @brief Get the size of the memory mapped area
     * 
     * @return std::size_t Size in bytes
     */
    XMIPP4_CORE_API std::size_t size() const noexcept;

    /**
     * @brief Get a pointer to the memory mapped area
     * 
     * @return void* Pointer to the data
     */
    XMIPP4_CORE_API void* data() const noexcept;

private:
    std::size_t m_size;
    void* m_data;

};

/**
 * @brief Exchange two memory_mapped_file objects
 *
 */
XMIPP4_CORE_API void swap(memory_mapped_file& lhs, memory_mapped_file& rhs) noexcept;

} //namespace system
} //namespace xmipp4
