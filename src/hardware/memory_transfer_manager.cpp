// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/memory_transfer_manager.hpp>

#include <xmipp4/core/hardware/memory_transfer_backend.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include "host_memory/host_memory_transfer_backend.hpp"
#include "memory_transfer_key.hpp"

#include <vector>
#include <unordered_map>

namespace xmipp4
{
namespace hardware
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
        m_cache.clear(); // Invalidate cache

        return true;
    }

    std::shared_ptr<memory_transfer> create_transfer(
        const memory_resource& src,
        const memory_resource& dst
    )
    {
        const memory_transfer_key key(src, dst);
        const auto ite = m_cache.find(key);
        if (ite != m_cache.end())
        {
            XMIPP4_ASSERT( ite->second );
            return ite->second;
        }

        auto *backend = find_most_suitable_backend(src, dst);
        if (backend)
        {
            const auto transfer = backend->create_transfer(src, dst);
            m_cache.emplace(key, transfer);
            return transfer;
        }

        throw invalid_operation_error(
            "No backend supports the requested transfer."
        );
    }

private:
    std::vector<std::unique_ptr<memory_transfer_backend>> m_backends;
    std::unordered_map<
        memory_transfer_key, 
        std::shared_ptr<memory_transfer>
    > m_cache;

    const memory_transfer_backend* find_most_suitable_backend(
        const memory_resource& src,
        const memory_resource& dst
    ) const
    {
        std::pair<const memory_transfer_backend*, backend_priority> best(
            nullptr, backend_priority::unsupported
        );

        for (auto ite = m_backends.cbegin(); ite != m_backends.cend(); ++ite)
        {
            XMIPP4_ASSERT(*ite);
            const auto &backend = **ite;
            const auto priority = backend.get_suitability(src, dst);

            if (priority > best.second)
            {
                best = { &backend, priority };
            }
        }

        return best.first;
    }

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
    host_memory_transfer_backend::register_at(*this);
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
)
{
    if (!m_implementation)
    {
        throw invalid_operation_error(
            "No backends were registered."
        );
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

} // namespace hardware
} // namespace xmipp4
