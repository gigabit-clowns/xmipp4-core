// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_type.hpp"

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

} // namespace hardware
} // namespace xmipp4
