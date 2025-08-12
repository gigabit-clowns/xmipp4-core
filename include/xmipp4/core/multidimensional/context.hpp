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

namespace parallel
{

class parallel_executor;

} // namespace parallel

namespace multidimensional
{

class allocator;
class strided_layout_policy;

/**
 * @brief Defines the environment used in computations.
 * 
 * This class holds the set of helper classes used in computations. Is is 
 * generally provided to operators in order to define execution policies.
 * 
 */
class context
{
public:
    context();
    context(const context &other) = delete;
    context(context &&other) noexcept;
    ~context();

    context& operator=(const context &other) = delete;
    context& operator=(context &&other) noexcept;

    /**
     * @brief Set the current device for this context.
     * 
     * @param device The device for this context. Use nullptr for none.
     */
    void set_device(std::shared_ptr<compute::device> device) noexcept;

    /**
     * @brief Get the current device for this context.
     * 
     * @return compute::device* The current device.
     */
    compute::device* get_device() const noexcept;

    /**
     * @brief Set the current device_queue for this context.
     * 
     * @param queue The device_queue for this context. Use nullptr for none.
     */
    void set_device_queue(std::shared_ptr<compute::device_queue> queue) noexcept;

    /**
     * @brief Get the current device for this context.
     * 
     * @return compute::device_queue* The current device_queue.
     */
    compute::device_queue* get_device_queue() const noexcept;

    /**
     * @brief Set the current allocator for this context.
     * 
     * @param allocator The allocator for this context. Use nullptr for none.
     */
    void set_allocator(std::shared_ptr<allocator> allocator) noexcept;

    /**
     * @brief Get the current allocator for this context.
     * 
     * @return allocator* The current device_queue.
     */
    allocator* get_allocator() const noexcept;

    /**
     * @brief Set the current scratch allocator for this context.
     * 
     * @param allocator The scratch allocator for this context. 
     * Use nullptr for none.
     * 
     */
    void set_scratch_allocator(std::shared_ptr<allocator> allocator) noexcept;

    /**
     * @brief Get the current scratch allocator for this context.
     * 
     * Unlike the allocator provided by get_allocator, the allocator returned
     * by this method is meant to be used for transient allocations used to hold 
     * intermediate results during computations. It is fine if the scratch 
     * allocator aliases the "normal" allocator.
     * 
     * @return allocator* The current scratch allocator.
     */
    allocator* get_scratch_allocator() const noexcept;

    /**
     * @brief Set the current layout policy for this context.
     * 
     * @param policy The layout policy for this context. Use nullptr for none
     */
    void set_layout_policy(std::shared_ptr<strided_layout_policy> policy) noexcept;

    /**
     * @brief Get the current layout policy for this context.
     * 
     * @return strided_layout_policy* The current layout policy.
     */
    strided_layout_policy* get_layout_policy() const noexcept;

    /**
     * @brief Set the current parallel executor for this context.
     * 
     * @param parallel_executor The parallel_executor for this context.
     */
    void set_parallel_executor(std::shared_ptr<parallel::parallel_executor> parallel_executor) noexcept;

    /**
     * @brief Get the current parallel executor for this context.
     * 
     * @return parallel::parallel_executor* The current parallel executor.
     */
    parallel::parallel_executor* get_parallel_executor() const noexcept;


private:
    class implementation;
    std::unique_ptr<implementation> m_implementation;

    implementation& create_if_null();

};

const context& get_default_context() noexcept;

} // namespace multidimensional
} // namespace xmipp4
