// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/unified_copy.hpp>

#include <xmipp4/core/compute/device_queue.hpp>
#include <xmipp4/core/compute/device_buffer.hpp>
#include <xmipp4/core/compute/device_memory_allocator.hpp>
#include <xmipp4/core/compute/host_buffer.hpp>
#include <xmipp4/core/compute/host_memory_allocator.hpp>
#include <xmipp4/core/compute/unified_buffer.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_unified_copy final
    : public unified_copy
{
public:
    MAKE_MOCK3(transfer_copy, void(const host_buffer&, device_buffer&, device_queue&), override);
    MAKE_MOCK4(transfer_copy, void(const host_buffer&, device_buffer&, span<const copy_region>, device_queue&), override);
    MAKE_MOCK4(transfer, std::shared_ptr<device_buffer>(const std::shared_ptr<host_buffer>&, device_memory_allocator&, std::size_t, device_queue&), override);
    MAKE_MOCK4(transfer, std::shared_ptr<const device_buffer>(const std::shared_ptr<const host_buffer>&, device_memory_allocator&, std::size_t, device_queue&), override);

    MAKE_MOCK3(transfer_copy, void(const device_buffer&, host_buffer&, device_queue&), override);
    MAKE_MOCK4(transfer_copy, void(const device_buffer&, host_buffer&, span<const copy_region>, device_queue&), override);
    MAKE_MOCK4(transfer, std::shared_ptr<host_buffer>(const std::shared_ptr<device_buffer>&, host_memory_allocator&, std::size_t, device_queue&), override);
    MAKE_MOCK4(transfer, std::shared_ptr<const host_buffer>(const std::shared_ptr<const device_buffer>&, host_memory_allocator&, std::size_t, device_queue&), override);

    MAKE_MOCK3(copy, void(const device_buffer&, device_buffer&, device_queue&), override);
    MAKE_MOCK4(copy, void(const device_buffer&, device_buffer&, span<const copy_region>, device_queue&), override);

    MAKE_MOCK3(copy_unified, void(const unified_buffer&, unified_buffer&, device_queue&), override);
    MAKE_MOCK4(copy_unified, void(const unified_buffer&, unified_buffer&, span<const copy_region>, device_queue&), override);

};

} // namespace compute
} // namespace xmipp4
