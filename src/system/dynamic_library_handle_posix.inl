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
 * @file dynamic_library_handle_posix.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief POSIX implementation of dynamic_library_handle.hpp
 * @date 2023-08-13
 * 
 */

#include "dynamic_library_handle.hpp"

#include <xmipp4/core/platform/constexpr.hpp>

#include <dlfcn.h>

#include <stdexcept>
#include <sstream>

namespace xmipp4
{
namespace system
{

inline void* dynamic_library_open(const char* filename)
{
    XMIPP4_CONST_CONSTEXPR int flags = RTLD_LAZY;
    auto *const result = ::dlopen(filename, flags);
    if (result == NULL)
    {
        std::ostringstream oss;
        oss << "Error loading dynamic library: " << dlerror();
        throw std::runtime_error(oss.str());
    }
    return result;
}

inline void dynamic_library_close(void* handle) noexcept
{
    ::dlclose(handle);
}

inline void* dynamic_library_get_symbol(void* handle, const char* name) noexcept
{
    return ::dlsym(handle, name);
}

inline std::string dynamic_library_symbol_filename_lookup(const void* symbol)
{
    std::string result;

    Dl_info info;
    if (dladdr(symbol, &info))
    {
        result = std::string(info.dli_fname);
    }

    return result;
}

} // namespace system
} // namespace xmipp4
