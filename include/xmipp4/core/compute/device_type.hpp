// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/constexpr.hpp"

#include <string_view>

namespace xmipp4
{
namespace compute
{

/**
 * @brief Device categories.
 * 
 */
enum class device_type
{
    unknown = -1, ///< The device does fall into any of the following categories
    cpu, ///< CPU of the host.
    gpu, ///< A discrete Graphics Processing Unit (GPU)
    integrated_gpu, ///< An integrated Graphics Processing Unit (iGPU)
    fpga, ///< A Field Programmable Grid Array (FPGA)
};

XMIPP4_CONSTEXPR const char* to_string(device_type type) noexcept;
bool from_string(std::string_view str, device_type& type) noexcept;

} // namespace compute
} // namespace xmipp4

#include "device_type.inl"
