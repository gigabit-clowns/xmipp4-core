#pragma once

/***************************************************************************
 * Authors:     Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 *
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
 * @file dynamic_library.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of dynamic_library class
 * @date 2023-07-09
 */

#include <string>
#include <cstdint>

namespace xmipp4
{

/**
 * @brief RAII style class to load, query and unload
 * dynamic libraries
 * 
 */
class dynamic_library
{
public:
    /**
     * @brief Construct an empty dynamic_library object
     * 
     */
    dynamic_library() noexcept;

    /**
     * @brief Construct a new dynamic_library object from a file
     * 
     * @param filename Path of the dynamic library file
     */
    explicit dynamic_library(const char* filename);

    /**
     * @brief Construct a new dynamic_library object from a file
     * 
     * @param filename Path of the dynamic library file
     */
    explicit dynamic_library(const std::string& filename);

    dynamic_library(const dynamic_library& other) = delete;
    dynamic_library(dynamic_library&& other) noexcept;
    ~dynamic_library();

    dynamic_library& operator=(const dynamic_library& other) = delete;
    dynamic_library& operator=(dynamic_library&& other) noexcept;

    void swap(dynamic_library& other) noexcept;

    operator bool() const noexcept;

    /**
     * @brief Loads a dynamic library
     * 
     * @param filename Path to the dynamic library
     */
    void open(const char* filename);

    /**
     * @brief Loads a dynamic library
     * 
     * @param filename Path to the dynamic library
     */
    void open(const std::string& filename);

    /**
     * @brief Frees the stored dynamic library, if any
     * 
     */
    void close() noexcept;

    /**
     * @brief Get the address of a symbol
     * 
     * @param name Name of the symbol
     * @return void* Pointer to the symbol. nullptr if not found
     */
    void* get_symbol(const char* name) const noexcept; 

    /**
     * @brief Get the address of a symbol
     * 
     * @param name Name of the symbol
     * @return void* Pointer to the symbol. nullptr if not found
     */
    void* get_symbol(const std::string& name) const noexcept; 

private:
    void* m_handle;

};

void swap(dynamic_library& lhs, dynamic_library& rhs) noexcept;

} // namespace xmipp4
