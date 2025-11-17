// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/communication/communicator_manager.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/platform/assert.hpp>
#include <xmipp4/core/communication/dummy/dummy_communicator_backend.hpp>

#include "../find_most_suitable_backend.hpp"

#include <vector>
#include <algorithm>

namespace xmipp4
{
namespace communication
{

class communicator_manager::implementation
{
public:
    bool register_backend(
        std::unique_ptr<communicator_backend> backend
    )
    {
        if (!backend)
        {
            return false;
        }

        auto name = backend->get_name();
        bool inserted;
        std::tie(std::ignore, inserted) = m_backends.emplace(
            std::move(name),
            std::move(backend)
        );

        return inserted;
    }

    std::shared_ptr<communicator> create_world_communicator() const
    {
        const auto backend = find_most_suitable_backend(
            m_backends.cbegin(), m_backends.cend(),
            [] (const auto &item)
            {
                XMIPP4_ASSERT(item.second);
                return item.second->get_suitability();
            }
        );

        if (backend == m_backends.cend())
        {
            throw invalid_operation_error(
                "There is no available device communicator backend"
            );
        }

        return backend->second->create_world_communicator();
    }

private:
    using backend_map = 
        std::unordered_map<std::string, std::unique_ptr<communicator_backend>>;
    backend_map m_backends;

};




communicator_manager::communicator_manager() = default;

communicator_manager::communicator_manager(
    communicator_manager &&other
) noexcept = default;

communicator_manager::~communicator_manager() = default;

communicator_manager& 
communicator_manager::operator=(communicator_manager&&) noexcept = default;

void communicator_manager::register_builtin_backends()
{
    dummy_communicator_backend::register_at(*this);
}

bool communicator_manager::register_backend(
    std::unique_ptr<communicator_backend> backend
)
{
    create_implementation_if_null();
    return m_implementation->register_backend(std::move(backend));
}

std::shared_ptr<communicator> 
communicator_manager::create_world_communicator() const
{
    if (!m_implementation)
    {
        throw invalid_operation_error(
            "No backends were registered."
        );
    }

    return m_implementation->create_world_communicator();
}

void communicator_manager::create_implementation_if_null()
{
    if (!m_implementation)
    {
        m_implementation = std::make_unique<implementation>();
    }
}

} // namespace communication
} // namespace xmipp4
