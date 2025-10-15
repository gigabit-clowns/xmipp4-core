// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../memory_resource.hpp"

namespace xmipp4 
{
namespace compute
{

class cpu_memory_resource final
    : public memory_resource
{
public:
    memory_resource_kind get_kind() const noexcept override;

    std::shared_ptr<memory_allocator> create_allocator() override;

}; 

} // namespace compute
} // namespace xmipp4
