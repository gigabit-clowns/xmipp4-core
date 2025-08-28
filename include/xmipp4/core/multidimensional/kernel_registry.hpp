// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "kernel_key.hpp"

#include <typeinfo>
#include <memory>
#include <functional>

namespace xmipp4 
{
namespace compute
{

class device_backend;

} // namespace compute

namespace multidimensional
{

class kernel;
class kernel_builder;
class kernel_iteration_layout;

class kernel_registry
{
public:
    kernel_registry();
    kernel_registry(const kernel_registry &other) = delete;
    kernel_registry(kernel_registry &&other) noexcept;
    ~kernel_registry();

    kernel_registry& operator=(const kernel_registry &other) = delete;
    kernel_registry& operator=(kernel_registry &&other) noexcept;

    bool register_kernel_builder(kernel_key key,
                                 const compute::device_backend &backend,
                                 std::unique_ptr<kernel_builder> builder );

    const kernel_builder * 
    get_kernel_builder(kernel_key key,
                       const compute::device_backend &backend ) const noexcept;

private:
    class implementation;
    std::unique_ptr<implementation> m_implementation;

    implementation& create_if_null();

};

} // namespace multidimensional
} // namespace xmipp4
