#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

template <typename... Slices>
class slice_sequence;

template <>
class slice_sequence<>
{
};

template <typename Head, typename... Tail>
class slice_sequence<Head, Tail...>
    : private slice_sequence<Tail...>
{
public:
    using head_type = Head;
    using tail_type = slice_sequence<Tail...>;

    slice_sequence() = default;
    template<typename First, typename... Rest>
    explicit XMIPP4_CONSTEXPR slice_sequence(First&& first, Rest&&... rest);
    slice_sequence(const slice_sequence &other) = default;
    slice_sequence(slice_sequence &&other) = default;
    ~slice_sequence() = default;

    slice_sequence& operator=(const slice_sequence &other) = default;
    slice_sequence& operator=(slice_sequence &&other) = default;

    XMIPP4_CONSTEXPR head_type& get() noexcept;
    XMIPP4_CONSTEXPR const head_type& get() const noexcept;
    XMIPP4_CONSTEXPR tail_type& tail() noexcept;
    XMIPP4_CONSTEXPR const tail_type& tail() const noexcept;

private:
    head_type m_value;

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

template <typename... Slices>
XMIPP4_CONSTEXPR
slice_sequence<Slices...> make_slice_sequence(const Slices&... slices);

} // namespace multidimensional
} // namespace xmipp4

#include "slice_sequence.inl"
