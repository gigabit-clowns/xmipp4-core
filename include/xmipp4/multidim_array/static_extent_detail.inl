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

/**
 * @file static_extent_detail.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation details static_extent.hpp
 * @date 2024-02-22
 * 
 */

#include "static_extent.hpp"

#include "../dynamic_size.hpp"
#include "../platform/attributes.hpp"

#include <stdexcept>

namespace xmipp4
{
namespace detail
{

/**
 * @brief Specialization for terminating the inheritance chain
 * 
 * @tparam i Index
 */
template<std::size_t i>
class static_extent_impl<i, std::integer_sequence<std::size_t>>
{
};

/**
 * @brief Specialization for dynamic size
 * 
 * @tparam i Index
 * @tparam Tail Size of the remaining dimensions
 */
template<std::size_t i, std::size_t... Tail>
class static_extent_impl<i, std::integer_sequence<std::size_t, dynamic_size, Tail...>>
    : public static_extent_impl<i+1, std::integer_sequence<std::size_t, Tail...>>
{
public:
    template<typename... Args>
    XMIPP4_CONSTEXPR static_extent_impl(std::size_t size, Args&&... args) noexcept
        : static_extent_impl<i+1, std::integer_sequence<std::size_t, Tail...>>(std::forward<Args>(args)...)
        , m_size(size)
    {
    }

    XMIPP4_CONSTEXPR std::size_t get_size() const noexcept
    {
        return m_size;
    }

private:
    std::size_t m_size;

};

/**
 * @brief Specialization for static size
 * 
 * @tparam i Index
 * @tparam Head Size of the current dimension
 * @tparam Tail Size of the remaining dimensions
 */
template<std::size_t i, std::size_t Head, std::size_t... Tail>
class static_extent_impl<i, std::integer_sequence<std::size_t, Head, Tail...>>
    : public static_extent_impl<i+1, std::integer_sequence<std::size_t, Tail...>>
{
public:
    template<typename... Args>
    XMIPP4_CONSTEXPR static_extent_impl(Args&&... args) noexcept
        : static_extent_impl<i+1, std::integer_sequence<std::size_t, Tail...>>(std::forward<Args>(args)...)
    {
    }

    static XMIPP4_CONSTEXPR std::size_t get_size() const noexcept
    {
        return Head;
    }

};

template<std::size_t i, std::size_t Head, std::size_t... Tail>
XMIPP4_INLINE_CONSTEXPR std::size_t
get(const static_extent_impl<i, std::integer_sequence<std::size_t, Head, Tail...>>& impl) noexcept
{
    return impl.get_size();
}

} // namespace detail
} // namespace xmipp4
