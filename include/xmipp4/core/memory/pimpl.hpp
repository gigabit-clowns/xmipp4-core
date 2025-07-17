// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/constexpr.hpp"
#include "../platform/attributes.hpp"

#include <memory>
#include <type_traits>

namespace xmipp4 
{
namespace memory
{

struct defer_construct_tag {};
XMIPP4_INLINE_CONST_CONSTEXPR defer_construct_tag defer_construct;

template <typename T, typename Alloc = std::allocator<T>>
class pimpl
{
public:
    using value_type = T;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    /**
     * @brief Deferred construction. 
     * 
     * The resulting object does not hold a value.
     * 
     */
    explicit pimpl(defer_construct_tag) noexcept;

    /**
     * @brief Deferred construction with custom allocator.
     * 
     * The resulting object does not hold a value.
     * 
     * @param alloc The allocator used for this object.
     */
    pimpl(defer_construct_tag, 
          const allocator_type& alloc ) noexcept;
    
    /**
     * @brief Construct an underlying value.
     * 
     * @tparam Args Argument types used for constructing the underlying value.
     * @param args Argument used for constructing the underlying value.
     */
    template <typename... Args, typename = typename std::enable_if<std::is_constructible<value_type, Args...>::value>::type>
    explicit pimpl(Args&&... args);

    /**
     * @brief Construct an underlying value and custom allocator.
     * 
     * @tparam Args Argument types used for constructing the underlying value.
     * @param alloc Allocator used for this object.
     * @param args Argument used for constructing the underlying value.
     */
    template <typename... Args, typename = typename std::enable_if<std::is_constructible<value_type, Args...>::value>::type>
    pimpl(std::allocator_arg_t, const allocator_type& alloc, Args&&... args);

    pimpl(const pimpl &other);
    pimpl(const pimpl &other, const allocator_type &alloc);
    pimpl(pimpl &&other) noexcept;
    ~pimpl();

    pimpl& operator=(const pimpl& other);
    pimpl& operator=(pimpl&& other) noexcept;

    /**
     * @brief Dereference operator.
     * 
     * The object must hold a value. Otherwise the behaviour is undefined.
     * 
     * @return reference Reference to the underlying object.
     */
    reference operator*() noexcept;

    /**
     * @brief Dereference operator.
     * 
     * The object must hold a value. Otherwise the behaviour is undefined.
     * 
     * @return reference Reference to the underlying object.
     */
    const_reference operator*() const noexcept;

    /**
     * @brief Dereference operator.
     * 
     * The object must hold a value. Otherwise the behaviour is undefined.
     * 
     * @return pointer Pointer to the underlying object.
     */
    pointer operator->() noexcept;

    /**
     * @brief Dereference operator.
     * 
     * The object must hold a value. Otherwise the behaviour is undefined.
     * 
     * @return pointer Pointer to the underlying object.
     */
    const_pointer operator->() const noexcept;

    /**
     * @brief Check if the object holds a value.
     * 
     * @return bool True if the object holds a value. False otherwise.
     */
    operator bool() const noexcept;

    /**
     * @brief Swap values with another object.
     * 
     * @param other The other object to be swapped.
     */
    void swap(pimpl& other) noexcept;

    /**
     * @brief Destroy the underlying object.
     * 
     */
    void reset() noexcept;

    /**
     * @brief Construct a new underlying object.
     * 
     * If the object already holds a value, this is destroyed before emplacing
     * a new one.
     * 
     * @tparam Args Argument types used for construction.
     * @param args Arguments used for construction.
     */
    template <typename... Args>
    void emplace(Args&&... args);

    /**
     * @brief Get a pointer to he underlying value.
     * 
     * @return pointer Pointer to the underlying value. nullptr if it does not 
     * hold a value.
     */
    pointer get() noexcept;

    /**
     * @brief Get a pointer to he underlying value.
     * 
     * @return pointer Pointer to the underlying value. nullptr if it does not 
     * hold a value.
     */
    const_pointer get() const noexcept;

    /**
     * @brief Get the allocator.
     * 
     * @return allocator_type The allocator.
     */
    allocator_type get_allocator() const noexcept;

private:
    using allocator_traits = std::allocator_traits<allocator_type>;

    XMIPP4_NO_UNIQUE_ADDRESS allocator_type m_allocator;
    pointer m_responsibility;

    void swap_responsibility(pimpl& other) noexcept;
    void move_assign(pimpl& other, std::false_type);
    void move_assign(pimpl& other, std::true_type);

};

template <typename T, typename Alloc>
void swap(pimpl<T, Alloc>& x, pimpl<T, Alloc>& y) noexcept;

} // namespace memory
} // namespace xmipp4

#include "pimpl.inl"
