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

#if XMIPP4_LINUX
    #include <unistd.h>
    #include <sys/sysinfo.h>
#elif XMIPP4_APPLE
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <mach/mach.h>
#elif XMIPP4_WINDOWS
    #include <Winsock.h>
    #include <WinBase.h>
#endif

#include <stdexcept>
#include <array>

namespace xmipp4
{
namespace system
{

std::string get_hostname()
{
    std::array<char, 512> hostname;

    #if XMIPP4_POSIX
        gethostname(hostname.data(), hostname.size());
    #elif XMIPP4_WINDOWS
        gethostname(hostname.data(), static_cast<int>(hostname.size()));
    #else
        #pragma message ("Cannot determine hostname for this platform")
        return "";
    #endif
    
    return std::string(hostname.data());
}

std::size_t get_total_system_memory()
{
    #if XMIPP4_LINUX
        struct sysinfo info;
        sysinfo(&info);
        return info.totalram * info.mem_unit;

    #elif XMIPP4_APPLE
        int mib[2] = { CTL_HW, HW_MEMSIZE };
        int64_t total_memory;
        size_t length = sizeof(total_memory);
        
        if (sysctl(mib, 2, &total_memory, &length, NULL, 0) != 0) 
        {
            throw std::runtime_error("sysctl failed");
        }

        return total_memory;

    #elif XMIPP4_WINDOWS
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullTotalPhys;

    #else
        #pragma message ("Cannot get total memory for this platform")
        return 0;
    #endif
}

std::size_t get_available_system_memory()
{
    #if XMIPP4_LINUX
        struct sysinfo info;
        sysinfo(&info);
        return info.freeram * info.mem_unit;

    #elif XMIPP4_APPLE
        // Get free and inactive memory using mach API
        vm_statistics64_data_t vm_info;
        mach_msg_type_number_t info_count = HOST_VM_INFO64_COUNT;
        int err = host_statistics64(
            mach_host_self(), 
            HOST_VM_INFO64, 
            static_cast<host_info64_t*>(&vm_info), 
            &info_count
        );
        if (err != KERN_SUCCESS) 
        {
            throw std::runtime_error("could not retrieve host statistics");
        }

        // Calculate available memory as free + inactive pages
        const auto page_size = getpagesize();
        const auto free_memory = vm_info.free_count * page_size;
        const auto inactive_memory = vm_info.inactive_count * page_size;
        return free_memory + inactive_memory; // Available memory in bytes

    #elif XMIPP4_WINDOWS
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullAvailPhys;

    #else
        #pragma message ("Cannot get available memory for this platform")
        return 0;
    #endif
}

} // namespace system
} // namespace xmipp4
