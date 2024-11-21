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
 * @file dynamic_library_handle_windows.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Windows implementation of dynamic_library_handle.hpp
 * @date 2023-08-13
 * 
 */

#include "dynamic_library_handle.hpp"

#include <windows.h>

#include <system_error>

namespace xmipp4
{
namespace system
{

inline void* dynamic_library_open(const char* filename)
{
    const auto result = static_cast<void*>(::LoadLibrary(filename));
    if (result == NULL)
    {
        throw std::system_error(
            std::error_code(::GetLastError(), std::system_category())
        );
    }
    return result;
}

inline void dynamic_library_close(void* handle) noexcept
{
    ::FreeLibrary(static_cast<HMODULE>(handle));
}

inline void* dynamic_library_get_symbol(void* handle, const char* name) noexcept
{
    return ::GetProcAddress(static_cast<HMODULE>(handle), name);
}

inline std::string dynamic_library_symbol_filename_lookup(const void* symbol)
{
    std::string result;

    HMODULE module;
    XMIPP4_CONST_CONSTEXPR DWORD flags = 
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT ;

    if (::GetModuleHandleEx(flags, reinterpret_cast<LPCTSTR>(symbol), &module)) 
    {
        char path[MAX_PATH];
        int count;
        if((count = ::GetModuleFileNameA(module, path, MAX_PATH)))
        {
            result = std::string(path, count);
        }
    }

    return result;
}

} // namespace system
} // namespace xmipp4
