// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

namespace xmipp4 
{
namespace compute
{

class device;
class device_queue;

} // namespace

namespace multidimensional
{

class allocator;

class context
{
public:
    context();
    context(const context &other) = delete;
    context(context &&other) noexcept;
    ~context();

    context& operator=(const context &other) = delete;
    context& operator=(context &&other) noexcept;

    void set_device(std::shared_ptr<compute::device> device) noexcept;

    compute::device* get_device() const noexcept;

    void set_device_queue(std::shared_ptr<compute::device_queue> queue) noexcept;

    compute::device_queue* get_device_queue() const noexcept;

    void set_allocator(std::shared_ptr<allocator> allocator) noexcept;

    allocator* get_allocator() const noexcept;

    void set_scratch_allocator(std::shared_ptr<allocator> allocator) noexcept;

    allocator* get_scratch_allocator() const noexcept;

private:
    class implementation;
    std::unique_ptr<implementation> m_implementation;

    void create_if_null();

};

} // namespace multidimensional
} // namespace xmipp4
