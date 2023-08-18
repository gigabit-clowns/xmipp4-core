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
namespace detail
{

inline std::size_t get_file_size(int fd) noexcept
{
    struct stat file_stats;
    fstat(fd, &file_stats);
    return static_cast<std::size_t>(file_stats.st_size);
}

inline int access_flags_to_open_flags(access_flags access)
{
    if (access == read_write) 
        return O_RDWR;
    else if (access == read_only) 
        return O_RDONLY;
    else if (access == write_only) 
        return O_WRONLY;
    else 
        throw std::runtime_error("Unsupported access");
}

inline int access_flags_to_mmap_prot_flags(access_flags access) noexcept
{
    int prot = PROT_NONE;
    if (access.test(access_flag_bits::read)) prot |= PROT_READ;
    if (access.test(access_flag_bits::write)) prot |= PROT_WRITE;
    return prot;
}

inline void* memory_mapped_file_open(const char* filename, 
                                     std::ptrdiff_t offset,
                                     std::size_t &size, 
                                     access_flags access )
{
    // Open the file descriptor
    const auto open_flags = access_flags_to_open_flags(access);
    const auto fd = open(filename, open_flags);
    if (fd < 0) {
        std::ostringstream oss;
        oss << "Error opening file: " << std::strerror(errno);
        throw std::runtime_error(oss.str());
    }
    
    // Populate the size if necessary
    if (size == memory_mapped_file::whole_file)
    {
        size = get_file_size(fd);
        if(size >= offset)
        {
            size -= offset;
        }
        else
        {
            throw std::runtime_error("Offset can not be greater than the file size");
        }
    }

    // Memory map the opened file descriptor
    const auto prot = access_flags_to_mmap_prot_flags(access); 
    const auto flags = MAP_SHARED;
    const auto addr = mmap(
        nullptr,
        size,
        prot,
        flags,
        fd,
        offset
    );

    // Close the file descriptor after memory mapping regardless
    // of the result
    close(fd);

    // Check the result
    if (addr == MAP_FAILED)
    {
        std::ostringstream oss;
        oss << "Error memory mapping the file: " << std::strerror(errno);
        throw std::runtime_error(oss.str());
    }

    return addr;
}

inline void memory_mapped_file_close(void* data, std::size_t size) noexcept
{
    munmap(data, size);
}

} // namespace detail
} // namespace xmipp4

