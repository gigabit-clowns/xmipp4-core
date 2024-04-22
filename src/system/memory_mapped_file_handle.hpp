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

#include <xmipp4/core/access_flags.hpp>
#include <xmipp4/core/platform/operating_system.h>

namespace xmipp4
{
namespace system
{

/**
 * @brief Memory map a file.
 * 
 * @param filename Path to the file to be mapped.
 * @param access Memory access type. It can not be empty.
 * @param size Number of bytes to be mapped. If zero, it 
 * will be written with the size of the file.
 * @param copy_on_write If true, changes are not commited 
 * to the file. Has no effect if write access is not enabled.
 * @return void* Pointer to the mapped data.
 */
void* memory_mapped_file_open(const char* filename, 
                              access_flags access,
                              std::size_t &size,
                              bool copy_on_write );

/**
 * @brief Close a memory mapped file.
 * 
 * @param data Pointer to the memory mapped data.
 * @param size Size of the memory mapped data.
 */
void memory_mapped_file_close(void* data, std::size_t size) noexcept;


} // namespace system
} // namespace xmipp4

#if XMIPP4_CHECK_OS(POSIX)
    #include "memory_mapped_file_handle_posix.inl"
#elif XMIPP4_CHECK_OS(WINDOWS)
    #include "memory_mapped_file_handle_windows.inl"
#else
    #error "No memory_mapped_file_handle implementation available for this platform"
#endif