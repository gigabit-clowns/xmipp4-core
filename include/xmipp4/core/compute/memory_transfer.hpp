// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

class buffer;
class device_queue;



class XMIPP4_CORE_API memory_transfer
{
public:
    memory_transfer() = default;
    memory_transfer(const memory_transfer &other) = default;
    memory_transfer(memory_transfer &&other) = default;
    virtual ~memory_transfer() = default;

    memory_transfer& operator=(const memory_transfer &other) = default;
    memory_transfer& operator=(memory_transfer &&other) = default;

    virtual
    void transfer(
        const buffer &source,
        buffer &destination,
        device_queue &queue
    ) = 0;

}; 

} // namespace compute
} // namespace xmipp4
