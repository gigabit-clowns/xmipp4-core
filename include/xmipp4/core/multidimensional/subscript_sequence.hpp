// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/attributes.hpp"
#include "../platform/constexpr.hpp"

#include <type_traits>
#include <cstddef>

namespace xmipp4 
{

template <typename T, typename>
struct is_index;

template <typename T, typename>
struct is_slice;

namespace multidimensional
{

/**
 * @brief Defines a sequence of subscript classes.
 * 
 * @tparam Subscripts Subscript classes. May be any specialization of slice, 
 * an integral value, std::integral_constant, new_axis_tag or ellipsis_tag.
 * 
 */
template <typename... Subscripts>
class subscript_sequence;

template <>
class subscript_sequence<>
{
    /**
     * @brief Obtain the number of subscripts on the sequence.
     * 
     * @return std::size_t Subscript count.
     */
    static XMIPP4_CONSTEXPR std::size_t size() noexcept;

    /**
     * @brief Obtain the number of axes consumed by this sequence.
     * 
     * @return std::size_t Number of consumed axes.
     */
    static XMIPP4_CONSTEXPR std::size_t consumption() noexcept;

    /**
     * @brief Obtain the number of axes produced by this sequence.
     * 
     * @return std::size_t Number of produced axes.
     */
    static XMIPP4_CONSTEXPR std::size_t production() noexcept;

};

template <typename Head, typename... Tail>
class subscript_sequence<Head, Tail...>
{
public:
    using head_type = Head;
    using tail_type = subscript_sequence<Tail...>;

    subscript_sequence() = default;

    /**
     * @brief Construct an subscript_sequence using its values.
     * 
     * @tparam First Type of the first subscript.
     * @tparam Rest Types of the rest of the subscripts.
     * @param first First item on the sequence.
     * @param rest The rest of the items on the sequence.
     * 
     */
    template<typename First, typename... Rest>
    explicit XMIPP4_CONSTEXPR subscript_sequence(First&& first, Rest&&... rest);
    subscript_sequence(const subscript_sequence &other) = default;
    subscript_sequence(subscript_sequence &&other) = default;
    ~subscript_sequence() = default;

    subscript_sequence& operator=(const subscript_sequence &other) = default;
    subscript_sequence& operator=(subscript_sequence &&other) = default;

    /**
     * @brief Get a reference to the head item on the sequence.
     * 
     * @return head_type& Head element.
     */
    XMIPP4_CONSTEXPR head_type& head() noexcept;

    /**
     * @brief Get a const reference to the head item on the sequence.
     * 
     * @return const head_type& Head element.
     */
    XMIPP4_CONSTEXPR const head_type& head() const noexcept;

    /**
     * @brief Get a reference to the trailing elements of the sequence.
     * 
     * @return tail_type& Head element.
     */
    XMIPP4_CONSTEXPR tail_type& tail() noexcept;

    /**
     * @brief Get a const reference to the trailing elements of the sequence.
     * 
     * @return const tail_type& Head element.
     */
    XMIPP4_CONSTEXPR const tail_type& tail() const noexcept;

    /**
     * @brief Obtain the number of subscripts on the sequence.
     * 
     * @return std::size_t Subscript count.
     */
    static XMIPP4_CONSTEXPR std::size_t size() noexcept;

    /**
     * @brief Obtain the number of axes consumed by this sequence.
     * 
     * @return std::size_t Number of consumed axes.
     */
    static XMIPP4_CONSTEXPR std::size_t consumption() noexcept;

    /**
     * @brief Obtain the number of axes produced by this sequence.
     * 
     * @return std::size_t Number of produced axes.
     */
    static XMIPP4_CONSTEXPR std::size_t production() noexcept;

private:
    XMIPP4_NO_UNIQUE_ADDRESS head_type m_head;
    XMIPP4_NO_UNIQUE_ADDRESS tail_type m_tail;

};



/**
 * @brief Tag to express the creation a new axis of size one.
 * NumPy equivalent of numpy.newaxis() or None.
 * 
 */
struct new_axis_tag {};

/**
 * @brief Construct a new_axis_tag.
 * 
 * @return new_axis_tag Newly constructed tag.
 */
XMIPP4_CONSTEXPR
new_axis_tag new_axis() noexcept;

/**
 * @brief Tag to express the consumption of an arbitrary amount 
 * of axes until no slack is left on the right-hand dimensions.
 * NumPy equivalent of Ellipsis or "...".
 * 
 */
struct ellipsis_tag {};

/**
 * @brief Construct a ellipsis_tag.
 * 
 * @return ellipsis_tag Newly constructed tag.
 */
XMIPP4_CONSTEXPR
ellipsis_tag ellipsis() noexcept;



template <typename T, typename = void>
struct axis_consumption;

template <typename T>
struct axis_consumption<T, typename std::enable_if<is_index<T>::value>::type>
    : std::integral_constant<std::size_t, 1>
{
};

template <typename T>
struct axis_consumption<T, typename std::enable_if<is_slice<T>::value>::type>
    : std::integral_constant<std::size_t, 1>
{
};

template <>
struct axis_consumption<new_axis_tag>
    : std::integral_constant<std::size_t, 0>
{
};



template <typename T, typename = void>
struct axis_production;

template <typename T>
struct axis_production<T, typename std::enable_if<is_index<T>::value>::type>
    : std::integral_constant<std::size_t, 0>
{
};

template <typename T>
struct axis_production<T, typename std::enable_if<is_slice<T>::value>::type>
    : std::integral_constant<std::size_t, 1>
{
};

template <>
struct axis_production<new_axis_tag>
    : std::integral_constant<std::size_t, 1>
{
};





/**
 * @brief Construct a sequence of subscripts of arbitrary type.
 * 
 * @tparam Subscripts Types of the subscripts.
 * @param subscripts Sequence of subscripts.
 * @return subscript_sequence<typename std::decay<Subscripts>::type...>
 * Object representing all provided subscripts.
 */
template <typename... Subscripts>
XMIPP4_CONSTEXPR
subscript_sequence<typename std::decay<Subscripts>::type...> 
make_subscript_sequence(Subscripts&&... subscripts);

} // namespace multidimensional
} // namespace xmipp4

#include "subscript_sequence.inl"
