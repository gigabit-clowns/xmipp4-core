// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/memory_allocator_manager.hpp>

#include <xmipp4/core/hardware/memory_allocator_backend.hpp>

#include "caching_memory_allocator/caching_memory_allocator_backend.hpp"
#include "host_memory/host_memory_allocator_backend.hpp"

#include <vector>
#include <unordered_map>

namespace xmipp4
{
namespace hardware
{

class memory_allocator_manager::implementation
{
public:
    bool register_backend(
        std::unique_ptr<memory_allocator_backend> backend
    )
    {
        if (!backend)
        {
            return false;
        }

        m_backends.push_back(std::move(backend));

        return true;
    }

    std::shared_ptr<memory_allocator> create_memory_allocator(
        memory_resource &resource
    ) const
    {
        // TODO
    }

private:
    std::vector<std::unique_ptr<memory_allocator_backend>> m_backends;

};



memory_allocator_manager::memory_allocator_manager() noexcept = default;

memory_allocator_manager::memory_allocator_manager(
    memory_allocator_manager &&other
) noexcept = default;

memory_allocator_manager::~memory_allocator_manager() = default;

memory_allocator_manager&
memory_allocator_manager::operator=(
    memory_allocator_manager &&other
) noexcept = default;

void memory_allocator_manager::register_builtin_backends()
{
    caching_memory_allocator_backend::register_at(*this);
    host_memory_allocator_backend::register_at(*this);
}

bool memory_allocator_manager::register_backend(
    std::unique_ptr<memory_allocator_backend> backend
)
{
    create_implementation_if_null();
    return m_implementation->register_backend(std::move(backend));
}

std::shared_ptr<memory_allocator> 
memory_allocator_manager::create_memory_allocator(
    memory_resource &resource
) const
{
    if (!m_implementation)
    {
        return nullptr;
    }

    return m_implementation->create_memory_allocator(resource);
}

void memory_allocator_manager::create_implementation_if_null()
{
    if (!m_implementation)
    {
        m_implementation = std::make_unique<implementation>();
    }
}

} // namespace hardware
} // namespace xmipp4
