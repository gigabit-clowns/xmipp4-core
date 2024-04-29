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
 * @file uuid.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of uuid.hpp
 * @date 2024-04-29
 * 
 */

#include "uuid.hpp"

#include <algorithm>

namespace xmipp4 
{

XMIPP4_INLINE_CONSTEXPR uuid::uuid(const data_type &data)
    : m_data(data)
{
}

XMIPP4_INLINE_CONSTEXPR_CPP20 uuid::uuid(uint8_t data[16])
{
    std::copy(
        data, data + m_data.size(),
        m_data.begin()
    );
}

XMIPP4_INLINE_CONSTEXPR
const uuid::data_type& uuid::get_data() const noexcept
{
    return m_data;
}

XMIPP4_INLINE_CONSTEXPR
uuid::time_low_type uuid::get_time_low() const noexcept
{
    return {
        m_data[0],
        m_data[1],
        m_data[2],
        m_data[3]
    };
}

XMIPP4_INLINE_CONSTEXPR
uuid::time_mid_type uuid::get_time_mid() const noexcept
{
    return {
        m_data[4],
        m_data[5]
    };
}

XMIPP4_INLINE_CONSTEXPR
uuid::time_high_and_version_type uuid::get_time_high_and_version() const noexcept
{
    return {
        m_data[6],
        m_data[7]
    };
}

XMIPP4_INLINE_CONSTEXPR
uuid::clock_sequence_and_variant_type uuid::get_clock_sequence_and_variant() const noexcept
{
    return {
        m_data[8],
        m_data[9]
    };
}

XMIPP4_INLINE_CONSTEXPR
uuid::node_type uuid::get_node() const noexcept
{
    return {
        m_data[10],
        m_data[11],
        m_data[12],
        m_data[13],
        m_data[14],
        m_data[15]
    };
}





XMIPP4_INLINE_CONSTEXPR_CPP20
bool operator==(const uuid &lhs, const uuid &rhs) noexcept
{
    return lhs.get_data() == rhs.get_data();
}

XMIPP4_INLINE_CONSTEXPR_CPP20
bool operator!=(const uuid &lhs, const uuid &rhs) noexcept
{
    return lhs.get_data() != rhs.get_data();
}

XMIPP4_INLINE_CONSTEXPR_CPP20
bool operator<(const uuid &lhs, const uuid &rhs) noexcept
{
    return lhs.get_data() < rhs.get_data();
}

XMIPP4_INLINE_CONSTEXPR_CPP20
bool operator<=(const uuid &lhs, const uuid &rhs) noexcept
{
    return lhs.get_data() <= rhs.get_data();
}

XMIPP4_INLINE_CONSTEXPR_CPP20
bool operator>(const uuid &lhs, const uuid &rhs) noexcept
{
    return lhs.get_data() > rhs.get_data();
}

XMIPP4_INLINE_CONSTEXPR_CPP20
bool operator>=(const uuid &lhs, const uuid &rhs) noexcept
{
    return lhs.get_data() >= rhs.get_data();
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, const uuid& id)
{
    const auto &data = id.get_data();
    
    // Create a copy of OS with own formatting
    std::ostream os2(os.rdbuf()); 
    os2 << std::hex;

    XMIPP4_CONST_CONSTEXPR T separator = '-'; 
    XMIPP4_CONST_CONSTEXPR std::array<std::size_t, 5> counts = {4, 2, 2, 2, 6}; 
    auto input_iterator = data.cbegin();
    auto output_iterator = std::ostream_iterator<uint8_t>(os2);
    for(std::size_t i = 0; i < counts.size(); ++i)
    {
        if(i > 0) *(output_iterator++) = separator;
        
        const auto count = counts[i];
        output_iterator = std::copy_n(input_iterator, count, output_iterator);
        input_iterator = std::next(input_iterator, count);
    }

    return os;
}

} // namespace xmipp4
