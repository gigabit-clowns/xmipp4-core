// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/context.hpp>

#include <xmipp4/core/multidimensional/allocator.hpp>
#include <xmipp4/core/compute/device.hpp>
#include <xmipp4/core/compute/device_queue.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class context::implementation
{
public:
    void set_device(std::shared_ptr<compute::device> device) noexcept
    {
        m_device = std::move(device);
    }

    compute::device* get_device() const noexcept
    {
        return m_device.get();
    }

    void set_device_queue(std::shared_ptr<compute::device_queue> queue) noexcept
    {
        m_queue = std::move(queue);
    }

    compute::device_queue* get_device_queue() const noexcept
    {
        return m_queue.get();
    }

    void set_allocator(std::shared_ptr<allocator> allocator) noexcept
    {
        m_allocator = std::move(allocator);
    }

    allocator* get_allocator() const noexcept
    {
        return m_allocator.get();
    }

    void set_scratch_allocator(std::shared_ptr<allocator> allocator) noexcept
    {
        m_scratch_allocator = std::move(allocator);
    }

    allocator* get_scratch_allocator() const noexcept
    {
        return m_scratch_allocator.get();
    }

private:
    std::shared_ptr<compute::device> m_device;
    std::shared_ptr<compute::device_queue> m_queue;
    std::shared_ptr<allocator> m_allocator;
    std::shared_ptr<allocator> m_scratch_allocator;

};



context::context() = default;
context::context(context &&other) noexcept = default;
context::~context() = default;

context& context::operator=(context &&other) noexcept = default;

void context::set_device(std::shared_ptr<compute::device> device) noexcept
{
    create_if_null();
    m_implementation->set_device(std::move(device));
}

compute::device* context::get_device() const noexcept
{
    return m_implementation ? 
           m_implementation->get_device() : 
           nullptr ;
}

void context::set_device_queue(std::shared_ptr<compute::device_queue> queue) noexcept
{
    create_if_null();
    m_implementation->set_device_queue(std::move(queue));
}

compute::device_queue* context::get_device_queue() const noexcept
{
    return m_implementation ? 
           m_implementation->get_device_queue() : 
           nullptr ;
}

void context::set_allocator(std::shared_ptr<allocator> allocator) noexcept
{
    create_if_null();
    m_implementation->set_allocator(std::move(allocator));
}

allocator* context::get_allocator() const noexcept
{
    return m_implementation ? 
           m_implementation->get_allocator() : 
           nullptr ;
}

void context::set_scratch_allocator(std::shared_ptr<allocator> allocator) noexcept
{
    create_if_null();
    m_implementation->set_scratch_allocator(std::move(allocator));
}

allocator* context::get_scratch_allocator() const noexcept
{
    return m_implementation ? 
           m_implementation->get_scratch_allocator() : 
           nullptr ;
}

void context::create_if_null()
{
    if (!m_implementation)
    {
        m_implementation = std::make_unique<implementation>();
    }
}

} // namespace multidimensional
} // namespace xmipp4
