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

#include "platform/constexpr.hpp"
#include "binary/flagset.hpp"

#include <string_view>
#include <ostream>

namespace xmipp4
{

enum class example_flag_bits
{
    first = binary::bit(0),
    second = binary::bit(1),
    third = binary::bit(2),
};

using example_flags = binary::flagset<example_flag_bits>;

XMIPP4_CONSTEXPR const char* to_string(example_flag_bits ex) noexcept;
bool from_string(std::string_view str, example_flag_bits& ex) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, enumeration ex);

template<typename B>
struct flag_traits;

template<>
struct flag_traits<compute::example_flag_bits>
{
    using flagset_type = compute::example_flags;

    static inline XMIPP4_CONST_CONSTEXPR flagset_type all = 
    {
        example_flag_bits::first,
        example_flag_bits::second,
        example_flag_bits::third,
    };
};

} // namespace xmipp4

#include "example_flags.inl"
