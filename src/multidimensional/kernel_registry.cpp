// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/kernel_registry.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include "kernel_key.hpp"

#include <unordered_map>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_registry::implementation
{
public:
    template <typename F>
    bool register_kernel(const std::type_info &operation_key,
                         const std::type_info &backend_key,
                         F &&kernel )
    {
        bool inserted;
        std::tie(std::ignore, inserted) = m_kernels.emplace(
            kernel_key(operation_key, backend_key),
            std::forward<F>(kernel)
        );

        return inserted;
    }

    const kernel_function_type* 
    get_kernel(const std::type_info &operation_key,
               const std::type_info &backend_key ) const noexcept
    {
        const auto ite = m_kernels.find(kernel_key(operation_key, backend_key));
        if (ite != m_kernels.cend())
        {
            return &(ite->second);
        }

        return nullptr;
    }

private:
    std::unordered_map<kernel_key, kernel_function_type> m_kernels;

};





kernel_registry::kernel_registry() = default;

kernel_registry::kernel_registry(kernel_registry &&other) noexcept = default;

kernel_registry::~kernel_registry() = default;

kernel_registry& 
kernel_registry::operator=(kernel_registry &&other) noexcept = default;

bool kernel_registry::register_kernel(const std::type_info &operation_key,
                                      const std::type_info &backend_key,
                                      const kernel_function_type &kernel )
{
    return create_if_null().register_kernel(
        operation_key, 
        backend_key, 
        kernel
    );
}

bool kernel_registry::register_kernel(const std::type_info &operation_key,
                                      const std::type_info &backend_key,
                                      kernel_function_type &&kernel )
{
    return create_if_null().register_kernel(
        operation_key, 
        backend_key, 
        std::move(kernel)
    );
}

const kernel_registry::kernel_function_type* 
kernel_registry::get_kernel(const std::type_info &operation_key,
                            const std::type_info &backend_key ) const noexcept
{
    return m_implementation ? 
           m_implementation->get_kernel(operation_key, backend_key) :
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
