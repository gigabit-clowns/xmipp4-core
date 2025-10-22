// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/memory_transfer.hpp>

#include "host_memory_transfer.hpp"

namespace xmipp4
{
namespace compute
{

const memory_transfer& get_host_memory_transfer() noexcept
{
    return host_memory_transfer::get();
}

} // namespace compute
} // namespace xmipp4
