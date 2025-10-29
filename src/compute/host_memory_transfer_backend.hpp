// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/memory_transfer_backend.hpp>

#include "host_memory_transfer.hpp"

namespace xmipp4 
{
namespace compute
{

class host_memory_transfer_backend
    : public memory_transfer_backend
{
public:
    host_memory_transfer_backend();

    std::shared_ptr<memory_transfer> create_transfer(
        const memory_resource& src,
        const memory_resource& dst
    ) const override;

private:
    std::shared_ptr<host_memory_transfer> m_transfer;

};

} // namespace compute
} // namespace xmipp4
