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

#include "../version.hpp"
#include "../platform/dynamic_shared_object.h"

#include <string>
#include <cstdint>

namespace xmipp4
{
namespace system
{

/**
 * @brief RAII style class to load, query and unload
 * dynamic libraries
 * 
 */
class XMIPP4_CORE_API dynamic_library
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

    /**
     * @brief Check if a dynamic library is loaded
     * 
     * @return bool Returns true if a library is loaded
     */
    bool is_open() const noexcept;

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

    /**
     * @brief Obtain the platform specific soname for a library name
     * 
     * @param library_name Name of the library
     * @return The soname
     */
    static 
    std::string make_soname(const std::string& library_name);

    /**
     * @brief Obtain the platform specific soname for a library name
     * 
     * @param library_name Name of the library
     * @param ver Version of the library
     * @return The soname
     */
    static
    std::string make_soname(const std::string& library_name, version ver);

    /**
     * @brief Obtain the filename to the dynamic library from where the
     * given symbol has been loaded.
     * 
     * @param symbol The symbol to be looked up.
     * @return std::string The path to the dynamic library from where 
     * the symbol has been loaded. Empty if no matching symbol was found. 
     */
    static std::string query_symbol_filename(const void* symbol);

private:
    void* m_handle;

};

/**
 * @brief Exchange two dynamic_library objects
 */
XMIPP4_CORE_API void swap(dynamic_library& lhs, dynamic_library& rhs) noexcept;

} // namespace system
} // namespace xmipp4
