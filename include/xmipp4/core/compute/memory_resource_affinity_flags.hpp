// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../binary/flagset.hpp"

namespace xmipp4 
{
namespace compute
{

enum class memory_resource_affinity_flag_bits
{
    device_local = binary::bit(0),
    host_accessible = binary::bit(1),
    managed = binary::bit(2)
};

using memory_resource_affinity_flags = 
    binary::flagset<memory_resource_affinity_flag_bits>;

} // namespace compute
} // namespace xmipp4
