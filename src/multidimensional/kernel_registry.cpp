// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/kernel_registry.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include <unordered_map>
#include <typeindex>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_registry::implementation
{
public:
    bool register_kernel(const std::type_info &operation_key, 
                         std::unique_ptr<kernel_handle> kernel)
    {
        bool inserted = false;

        if (kernel)
        {
            std::tie(std::ignore, inserted) = m_kernels.emplace(
                operation_key,
                std::move(kernel)
            );
        }

        return inserted;
    }

    const kernel_handle* 
    get_kernel(const std::type_info &operation_key) const noexcept
    {
        const kernel_handle *result = nullptr;

        const auto ite = m_kernels.find(operation_key);
        if (ite != m_kernels.cend())
        {
            result = ite->second.get();
            XMIPP4_ASSERT( result );
        }

        return result;
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<kernel_handle>> m_kernels;

};





kernel_registry::kernel_registry() = default;

kernel_registry::kernel_registry(kernel_registry &&other) noexcept = default;

kernel_registry::~kernel_registry() = default;

kernel_registry& 
kernel_registry::operator=(kernel_registry &&other) noexcept = default;

bool kernel_registry::register_kernel(const std::type_info &operation_key,
                                      std::unique_ptr<kernel_handle> kernel )
{
    return create_if_null().register_kernel(
        operation_key, 
        std::move(kernel)
    );
}

const kernel_handle* 
kernel_registry::get_kernel(const std::type_info &operation_key) const noexcept
{
    return m_implementation ? 
           m_implementation->get_kernel(operation_key) :
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
