// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <typeinfo>
#include <vector>
#include <memory>
#include <functional>

namespace xmipp4 
{
namespace multidimensional
{

class array;

class kernel_registry
{
public:
    using kernel_function_type = std::function<void(std::vector<array>)>;

    kernel_registry();
    kernel_registry(const kernel_registry &other) = delete;
    kernel_registry(kernel_registry &&other) noexcept;
    ~kernel_registry();

    kernel_registry& operator=(const kernel_registry &other) = delete;
    kernel_registry& operator=(kernel_registry &&other) noexcept;

    bool register_kernel(const std::type_info &operation_key,
                         const std::type_info &backend_key,
                         const kernel_function_type &kernel );

    bool register_kernel(const std::type_info &operation_key,
                         const std::type_info &backend_key,
                         kernel_function_type &&kernel );
    
    const kernel_function_type* 
    get_kernel(const std::type_info &operation_key,
               const std::type_info &backend_key ) const noexcept;

private:
    class implementation;
    std::unique_ptr<implementation> m_implementation;

    implementation& create_if_null();

};

} // namespace multidimensional
} // namespace xmipp4
