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
 * @file host.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host.hpp
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/system/host.hpp>
#include <xmipp4/core/platform/operating_system.h>

#if XMIPP4_POSIX
    #include <unistd.h>
#elif XMIPP4_WINDOWS
    #include <Winsock.h>
    #include <WinBase.h>
#endif

namespace xmipp4
{
namespace system
{

std::string get_hostname()
{
    #if XMIPP4_POSIX
        char hostname[512];
        int count = 512;
        gethostname(hostname, count);
        return std::string(hostname);
    #elif XMIPP4_WINDOWS
        char hostname[MAX_COMPUTERNAME_LENGTH + 1];
        const int count = MAX_COMPUTERNAME_LENGTH + 1;
        gethostname(hostname, count);
        return std::string(hostname);
    #else
        #pragma message ("Cannot determine hostname for this platform")
        return "";
    #endif
}

std::size_t get_total_system_memory()
{
    #if XMIPP4_POSIX
        long pages = sysconf(_SC_PHYS_PAGES);
        long page_size = sysconf(_SC_PAGE_SIZE);
        return pages * page_size;
    #elif XMIPP4_WINDOWS
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullTotalPhys;
    #else
        #pragma message ("Cannot get total memory for this platform")
        return "";
    #endif
}

} // namespace system
} // namespace xmipp4
