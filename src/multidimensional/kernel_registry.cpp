// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/kernel_registry.hpp>

#include <xmipp4/core/multidimensional/kernel_builder.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/compute/device_backend.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include "kernel_key.hpp"

#include <unordered_map>
#include <typeindex>
#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_registry::implementation
{
public:
    bool register_kernel_builder(const std::type_info &operation_key, 
                                 const compute::device_backend &backend_key,
                                 std::unique_ptr<kernel_builder> builder )
    {
        bool inserted = false;

        if (builder)
        {
            std::tie(std::ignore, inserted) = m_builders.emplace(
                kernel_key(operation_key, &backend_key),
                std::move(builder)
            );
        }

        return inserted;
    }

    const kernel_builder* 
    get_kernel_builder(const std::type_info &operation_key,
                       const compute::device_backend &backend_key ) const noexcept
    {
        const kernel_builder *result = nullptr;

        const auto ite = m_builders.find(
            kernel_key(operation_key, &backend_key)
        );
        if (ite != m_builders.cend())
        {
            result = ite->second.get();
            XMIPP4_ASSERT( result );
        }

        return result;
    }

private:
    std::unordered_map<kernel_key, std::unique_ptr<kernel_builder>> m_builders;

};





kernel_registry::kernel_registry() = default;

kernel_registry::kernel_registry(kernel_registry &&other) noexcept = default;

kernel_registry::~kernel_registry() = default;

kernel_registry& 
kernel_registry::operator=(kernel_registry &&other) noexcept = default;

bool kernel_registry::register_kernel_builder(const std::type_info &operation_key,
                                              const compute::device_backend &backend_key,
                                              std::unique_ptr<kernel_builder> builder )
{
    return create_if_null().register_kernel_builder(
        operation_key, 
        backend_key,
        std::move(builder)
    );
}

const kernel_builder* 
kernel_registry::get_kernel_builder(const std::type_info &operation_key,
                                    const compute::device_backend &backend_key ) const noexcept
{
    return m_implementation ? 
           m_implementation->get_kernel_builder(operation_key, backend_key) :
           nullptr ;
}

kernel_registry::implementation& kernel_registry::create_if_null()
{
    if (!m_implementation)
    {
        m_implementation = std::make_unique<implementation>();
    }

    XMIPP4_ASSERT( m_implementation );
    return *m_implementation;
}

} // namespace multidimensional
} // namespace xmipp4
