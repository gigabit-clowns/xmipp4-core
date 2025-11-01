// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_transfer_backend.hpp"

#include <xmipp4/core/hardware/memory_resource.hpp>

namespace xmipp4
{
namespace hardware
{

host_memory_transfer_backend::host_memory_transfer_backend()
    : m_transfer(std::make_shared<host_memory_transfer>())
{
}

std::shared_ptr<memory_transfer> host_memory_transfer_backend::create_transfer(
    const memory_resource& src,
    const memory_resource& dst
) const
{
    if (is_host_accessible(src.get_kind()) && is_host_accessible(dst.get_kind())) 
    {
        XMIPP4_ASSERT( m_transfer );
        return m_transfer;
    }

    return nullptr;
}


} // namespace hardware
} // namespace xmipp4
