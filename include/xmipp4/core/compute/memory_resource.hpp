// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "memory_resource_kind.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

class memory_allocator;
class device;



class XMIPP4_CORE_API memory_resource
{
public:
    memory_resource() = default;
    memory_resource(const memory_resource &other) = default;
    memory_resource(memory_resource &&other) = default;
    virtual ~memory_resource() = default;

    memory_resource& operator=(const memory_resource &other) = default;
    memory_resource& operator=(memory_resource &&other) = default;

    virtual
    memory_resource_kind get_kind() const noexcept = 0;

    virtual
    std::shared_ptr<memory_allocator> create_allocator() = 0;

}; 

} // namespace compute
} // namespace xmipp4
