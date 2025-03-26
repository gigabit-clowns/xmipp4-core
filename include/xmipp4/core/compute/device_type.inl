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

#include "device_type.hpp"

#include <unordered_map>

namespace xmipp4
{
namespace compute
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(device_type type) noexcept
{
    switch (type)
    {
    case device_type::unknown: return "unknown";
    case device_type::cpu: return "CPU";
    case device_type::gpu: return "GPU";
    case device_type::integrated_gpu: return "iGPU";
    case device_type::fpga: return "FPGA";
    default: return "";
    }
}

inline
bool from_string(std::string_view str, device_type& type) noexcept
{
    static const
    std::unordered_map<std::string_view, device_type> str_2_device_type = 
    {
        { to_string(device_type::unknown), device_type::unknown },
        { to_string(device_type::cpu), device_type::cpu },
        { to_string(device_type::gpu), device_type::gpu },
        { to_string(device_type::integrated_gpu), device_type::integrated_gpu },
        { to_string(device_type::fpga), device_type::fpga },
    };

    const auto ite = str_2_device_type.find(str);
    const bool result = ite != str_2_device_type.end();
    if(result)
        type = ite->second;
    return result;
}

} // namespace compute
} // namespace xmipp4
