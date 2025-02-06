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
 * @file layout_flags.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Declares layout_flags flagset
 * @date 2024-05-01
 * 
 */

#include "../utils/bit.hpp"
#include "../utils/flagset.hpp"
#include "../platform/constexpr.hpp"

#include <ostream>

namespace xmipp4 
{
namespace multidimensional
{

enum class layout_flag_bits
{
    contiguous = utils::bit(0),
    column_major = utils::bit(1),
    row_major = utils::bit(2)
};

using layout_flags = utils::flagset<layout_flag_bits>;

template <typename T>
std::basic_ostream<T>& 
operator<<(std::basic_ostream<T>& os, layout_flag_bits flag);

} // namespace multidimensional
} // namespace xmipp4

#include "layout_flags.inl"
