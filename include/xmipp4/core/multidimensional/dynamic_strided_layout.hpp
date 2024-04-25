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

#include "axis_descriptor.hpp"

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

class dynamic_strided_layout
{
public:
    using shape_type = std::vector<std::size_t>;

    dynamic_strided_layout() = default;
    template <typename It>
    dynamic_strided_layout(It first, It last);
    dynamic_strided_layout(std::initializer_list<std::size_t> shape);
    dynamic_strided_layout(const shape_type &shape);
    dynamic_strided_layout(const dynamic_strided_layout& other) = default;
    dynamic_strided_layout(dynamic_strided_layout&& other) = default;
    ~dynamic_strided_layout() = default;

    dynamic_strided_layout& operator=(const dynamic_strided_layout& other) = default;
    dynamic_strided_layout& operator=(dynamic_strided_layout&& other) = default;

    std::size_t get_rank() const noexcept;
    const axis_descriptor& get_axis(std::size_t i) const;
    std::size_t get_extent(std::size_t i) const;
    std::ptrdiff_t get_stride(std::size_t i) const;

    std::size_t get_required_storage_size() const noexcept;

private:
    std::vector<axis_descriptor> m_axes;
    std::ptrdiff_t m_offset;

};

template <typename T>
struct layout_traits;

template <>
struct layout_traits<dynamic_strided_layout>
{
    using shape_type = dynamic_strided_layout::shape_type;
};

} // namespace multidimensional
} // namespace xmipp4

#include "dynamic_strided_layout.inl"
