#pragma once

/***************************************************************************
 * Authors:     Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 *
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

#include "platform/constexpr.hpp"
#include "utils/bit.hpp"
#include "utils/flagset.hpp"

#include <ostream>

namespace xmipp4 
{

enum class access_flag_bits {
    read = utils::bit(0),
    write = utils::bit(1),
};

using access_flags = utils::flagset<access_flag_bits>;

inline XMIPP4_CONST_CONSTEXPR access_flags read_only(access_flag_bits::read);
inline XMIPP4_CONST_CONSTEXPR access_flags write_only(access_flag_bits::write);
inline XMIPP4_CONST_CONSTEXPR access_flags read_write({access_flag_bits::read, access_flag_bits::write});

XMIPP4_CONSTEXPR 
const char* to_string(access_flag_bits v) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, access_flag_bits v);

} // namespace xmipp4

#include "access_flags.inl"