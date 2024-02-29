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

#include "memory_mapped_file_handle.hpp"

#include <platform/constexpr.hpp>

#include <stdexcept>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace xmipp4
{
namespace system
{

inline int access_flags_to_open_flags(access_flags access)
{
    if (access == read_write) 
        return O_RDWR;
    else if (access == read_only) 
        return O_RDONLY;
    else if (access == write_only) 
        return O_WRONLY;
    else 
        throw std::invalid_argument("Unsupported access");
}

inline int access_flags_to_mmap_prot_flags(access_flags access) noexcept
{
    int prot = PROT_NONE;
    if (access.test(access_flag_bits::read)) prot |= PROT_READ;
    if (access.test(access_flag_bits::write)) prot |= PROT_WRITE;
    return prot;
}

inline int open_file(const char* filename, 
                     access_flags access )
{
    const auto open_flags = access_flags_to_open_flags(access);
    const auto fd = open(filename, open_flags);
    if (fd < 0) 
    {
        std::ostringstream oss;
        oss << "Error opening file: " << std::strerror(errno);
        throw std::runtime_error(oss.str());
    }
    return fd;
}

inline void* memory_map_file_descriptor(int fd, 
                                        access_flags access, 
                                        std::size_t size )
{
    const int prot = access_flags_to_mmap_prot_flags(access); 
    XMIPP4_CONST_CONSTEXPR int flags = MAP_SHARED;
    XMIPP4_CONST_CONSTEXPR off_t offset = 0;

    const auto result = mmap(
        nullptr,
        size,
        prot,
        flags,
        fd,
        offset
    );

    if (result == MAP_FAILED)
    {
        std::ostringstream oss;
        oss << "Error memory mapping the file: " << std::strerror(errno);
        throw std::runtime_error(oss.str());
    }

    return result;
}

inline std::size_t get_file_size(int fd) noexcept
{
    struct stat file_stats;
    fstat(fd, &file_stats);
    return static_cast<std::size_t>(file_stats.st_size);
}



inline void* memory_mapped_file_open(const char* filename, 
                                     access_flags access,
                                     std::size_t &size )
{
    const auto fd = open_file(filename, access);

    void* result;
    try
    {
        if (size == 0) size = get_file_size(fd);
        result = memory_map_file_descriptor(filename, access, size);
    }
    catch(...)
    {
        close(fd);
        throw;
    }
    
    // Close the file descriptor as it is not 
    // longer needed
    close(fd);

    return result;
}

inline void memory_mapped_file_close(void* data, std::size_t size) noexcept
{
    munmap(data, size);
}

} // namespace detail
} // namespace xmipp4

