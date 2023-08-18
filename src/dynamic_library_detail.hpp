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
 * @file dynamic_library_detail.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Platform independent functions for loading, unloading and querying
 * dynamic libraries
 * @date 2023-08-13
 * 
 */

#include <xmipp4/platform/operating_system.h>

namespace xmipp4
{
namespace detail
{

/**
 * @brief Open a the dynamic library
 * 
 * @param filename Path to the dynamic library
 * @return void* Pointer to the newly opened dynamic library
 */
void* dynamic_library_open(const char* filename);

/**
 * @brief Close a dynamic library
 * 
 * @param handle Handle of the dynamic library to be closed
 */
void dynamic_library_close(void* handle) noexcept;

/**
 * @brief Get a symbol from the dynamic library
 * 
 * @param handle Handle of the dynamic library
 * @param name Name of the symbol
 * @return void* Pointer to the queried symbol. NULL if not found
 */
void* dynamic_library_get_symbol(void* handle, const char* name) noexcept;

} // namespace detail
} // namespace xmipp4

#if defined(XMIPP4_POSIX)
    #include "dynamic_library_detail_posix.inl"
#elif defined(XMIPP4_WINDOWS)
    #include "dynamic_library_detail_windows.inl"
#else
    #error "No dynamic library implementation available for this platform"
#endif