// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/memory_transfer_manager.hpp>

#include <xmipp4/core/compute/memory_transfer_backend.hpp>

#include "host_memory_transfer.hpp"

#include <vector>
#include <unordered_map>

namespace xmipp4
{
namespace compute
{

class memory_transfer_manager::implementation
{
public:
    bool register_backend(
        std::unique_ptr<memory_transfer_backend> backend
    )
    {
        if (!backend)
        {
            return false;
        }

        m_backends.push_back(std::move(backend));
        return true;
    }

    std::shared_ptr<memory_transfer> create_transfer(
        const memory_resource& src,
        const memory_resource& dst
    ) const
    {
        // TODO retrieve from cache

        for (const auto& backend : m_backends)
        {
            const auto transfer = backend->create_transfer(src, dst);
            if (transfer)
            {
                return transfer;
            }
        }

        return nullptr;
    }

private:
    std::vector<std::unique_ptr<memory_transfer_backend>> m_backends;
    // TODO cache

};



memory_transfer_manager::memory_transfer_manager() noexcept = default;

memory_transfer_manager::memory_transfer_manager(
    memory_transfer_manager &&other
) noexcept = default;

memory_transfer_manager::~memory_transfer_manager() = default;

memory_transfer_manager&
memory_transfer_manager::operator=(
    memory_transfer_manager &&other
) noexcept = default;

void memory_transfer_manager::register_builtin_backends()
{
    // TODO
}

bool memory_transfer_manager::register_backend(
    std::unique_ptr<memory_transfer_backend> backend
)
{
    create_implementation_if_null();
    return m_implementation->register_backend(std::move(backend));
}

std::shared_ptr<memory_transfer> memory_transfer_manager::create_transfer(
    const memory_resource& src,
    const memory_resource& dst
) const
{
    if (!m_implementation)
    {
        return nullptr;
    }

    return m_implementation->create_transfer(src, dst);
}

void memory_transfer_manager::create_implementation_if_null()
{
    if (!m_implementation)
    {
        m_implementation = std::make_unique<implementation>();
    }
}

} // namespace compute
} // namespace xmipp4
