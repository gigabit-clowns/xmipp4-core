// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <array>
#include <ostream>
#include <cstddef>

#include "subscript_tags.hpp"
#include "slice.hpp"
#include "index.hpp"
#include "../platform/attributes.hpp"

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Class describing a runtime defined subscript.
 * 
 * A subscript may be either a
 * - ellipsis_tag
 * - new_axis_tag
 * - index (std::ptrdiff_t)
 * - slice (dynamic_slice)
 * 
 */
class dynamic_subscript
{
public:
    /**
     * @brief Enumeration holding the types this object can hold. 
     * 
     */
    enum class subscript_type
    {
        ellipsis,
        new_axis,
        index,
        slice
    };

    /**
     * @brief Construct a dynamic_subscript holding an ellipsis_tag.
     * 
     */
    XMIPP4_CONSTEXPR
    dynamic_subscript(ellipsis_tag) noexcept;

    /**
     * @brief Construct a dynamic_subscript holding a new_axis_tag.
     * 
     */
    XMIPP4_CONSTEXPR
    dynamic_subscript(new_axis_tag) noexcept;

    /**
     * @brief Construct a dynamic_subscript holding an index.
     * 
     * @tparam I Index type. 
     * @param index The index that is assigned to this object.

     */
    template <typename I, typename = typename std::enable_if<is_index<I>::value>::type>
    XMIPP4_CONSTEXPR
    dynamic_subscript(I index) noexcept;

    /**
     * @brief Construct a dynamic_subscript holding a slice.
     * 
     * @tparam Start Type of the start value.
     * @tparam Stop Type of the stop value.
     * @tparam Step Type of the step value.
     * @param slice the slice that is assigned to this object.
     * 
     */
    template <typename Start, typename Stop, typename Step>
    XMIPP4_CONSTEXPR
    dynamic_subscript(const slice<Start, Stop, Step> &slice) noexcept;

    dynamic_subscript(const dynamic_subscript &other) = default;
    dynamic_subscript(dynamic_subscript &&other) = default;
    ~dynamic_subscript() = default;

    dynamic_subscript& operator=(const dynamic_subscript &other) = default;
    dynamic_subscript& operator=(dynamic_subscript &&other) = default;

    /**
     * @brief Get the subscript type held by this object.
     * 
     * @return subscript_type 
     */
    XMIPP4_CONSTEXPR
    subscript_type get_subscript_type() const noexcept;

    /**
     * @brief Get the index.
     * 
     * This method shall only be called if get_subscript_type() returns
     * subscript_type::index. Otherwise an exception is thrown.
     * 
     * @return std::ptrdiff_t The index held by this object.
     * @throws std::logic_error
     */
    std::ptrdiff_t get_index() const;

    /**
     * @brief Get the slice.
     * 
     * This method shall only be called if get_subscript_type() returns
     * subscript_type::slice. Otherwise an exception is thrown.
     * 
     * @return dynamic_slice The slice held by this object.
     * @throws std::logic_error
     */
    dynamic_slice get_slice() const;

private:
    using storage_type = std::array<std::ptrdiff_t, 3>;

    storage_type m_data; // When holding index, first value is used.
    subscript_type m_type;

};



template <typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, 
                                  const dynamic_subscript &subscript );

/**
 * @brief Invoke the provided function with the value held by the dynamic 
 * subscript
 * 
 * @tparam F Function type.
 * @param func The function to be invoked. Must be callable with the following 
 * argument types:
 * - ellipsis_tag
 * - new_axis_tag
 * - std::ptrdiff_t
 * - dynamic_slice
 * 
 * @param subscript The subscript to be evaluated.
 * @return auto result of calling func with the evaluated argument.
 */
template <typename F>
XMIPP4_CONSTEXPR
auto visit(F&& func, const dynamic_subscript &subscript);

} // namespace multidimensional
} // namespace xmipp4

#include "dynamic_subscript.inl"
