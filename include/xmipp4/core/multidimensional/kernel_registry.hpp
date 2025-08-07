// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <typeinfo>
#include <memory>
#include <functional>

namespace xmipp4 
{
namespace multidimensional
{

class array_iterator;
class kernel_handle;
class context;

class kernel_registry
{
public:
    kernel_registry();
    kernel_registry(const kernel_registry &other) = delete;
    kernel_registry(kernel_registry &&other) noexcept;
    ~kernel_registry();

    kernel_registry& operator=(const kernel_registry &other) = delete;
    kernel_registry& operator=(kernel_registry &&other) noexcept;

    bool register_kernel(const std::type_info &operation_key,
                         std::unique_ptr<kernel_handle> kernel );

    const kernel_handle*
    get_kernel(const std::type_info &operation_key) const noexcept;

private:
    class implementation;
    std::unique_ptr<implementation> m_implementation;

    implementation& create_if_null();

};

} // namespace multidimensional
} // namespace xmipp4
