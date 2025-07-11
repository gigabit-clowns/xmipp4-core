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

/**
 * @file mock_layout.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of a layout class.
 * @date 2025-07-11
 */

#include <trompeloeil.hpp>

#include <xmipp4/core/multidimensional/dynamic_subscript.hpp>
#include <xmipp4/core/multidimensional/strided_axis.hpp>
#include <xmipp4/core/span.hpp>

#include <vector>

namespace xmipp4
{
namespace multidimensional
{

class mock_layout
{
public:
    static constexpr bool trompeloeil_movable_mock = true;

    MAKE_MOCK0(get_rank, std::size_t(), const noexcept);
    MAKE_MOCK3(get_axes, std::size_t(std::size_t*, std::ptrdiff_t*, std::size_t), const noexcept);
    MAKE_MOCK2(get_axis, bool(std::size_t, strided_axis&), const noexcept);
    MAKE_MOCK0(get_offset, std::ptrdiff_t(), const noexcept);
    MAKE_MOCK0(compute_storage_requirement, std::size_t(), const noexcept);
    MAKE_MOCK1(apply_subscripts, mock_layout(span<const dynamic_subscript>), const);
    MAKE_MOCK0(transpose, mock_layout(), const);
    MAKE_MOCK1(permute, mock_layout(span<const std::size_t>), const);
    MAKE_MOCK2(swap_axes, mock_layout(std::size_t, std::size_t), const);
    MAKE_MOCK0(squeeze, mock_layout(), const);
    MAKE_MOCK2(broadcast_extents_to_layout, void(std::vector<std::size_t>&, std::size_t), const);
    MAKE_MOCK2(broadcast_layout_to_extents, mock_layout(span<const std::size_t>, std::size_t), const);

};

} // namespace multidimensional
} // namespace xmipp4
