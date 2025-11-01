// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_type.hpp"

#include <unordered_map>

namespace xmipp4
{
namespace hardware
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

} // namespace hardware
} // namespace xmipp4
