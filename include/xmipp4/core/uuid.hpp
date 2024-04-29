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
 * @file uuid.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the uuid class
 * @date 2024-04-29
 * 
 */

#include "platform/constexpr.hpp"

#include <array>
#include <ostream>
#include <cstdint>
#include <random>

namespace xmipp4 
{

class uuid
{
public:
    using data_type = std::array<uint8_t, 16>;
    using time_low_type = std::array<uint8_t, 4>;
    using time_mid_type = std::array<uint8_t, 2>;
    using time_high_and_version_type = std::array<uint8_t, 2>;
    using clock_sequence_and_variant_type = std::array<uint8_t, 2>;
    using node_type = std::array<uint8_t, 6>;

    uuid() = default;
    XMIPP4_CONSTEXPR uuid(const data_type &data);
    XMIPP4_CONSTEXPR_CPP20 uuid(uint8_t data[16]);
    uuid(const uuid &other) = default;
    uuid(uuid &&other) = default;
    ~uuid() = default;
    
    uuid& operator=(const uuid &other) = default;
    uuid& operator=(uuid &&other) = default;
    
    XMIPP4_CONSTEXPR const data_type& get_data() const noexcept;
    XMIPP4_CONSTEXPR time_low_type get_time_low() const noexcept;
    XMIPP4_CONSTEXPR time_mid_type get_time_mid() const noexcept;
    XMIPP4_CONSTEXPR time_high_and_version_type get_time_high_and_version() const noexcept;
    XMIPP4_CONSTEXPR clock_sequence_and_variant_type get_clock_sequence_and_variant() const noexcept;
    XMIPP4_CONSTEXPR node_type get_node() const noexcept;

private:
    data_type m_data;

};

XMIPP4_CONSTEXPR_CPP20 bool operator==(const uuid &lhs, const uuid &rhs) noexcept;
XMIPP4_CONSTEXPR_CPP20 bool operator!=(const uuid &lhs, const uuid &rhs) noexcept;
XMIPP4_CONSTEXPR_CPP20 bool operator<(const uuid &lhs, const uuid &rhs) noexcept;
XMIPP4_CONSTEXPR_CPP20 bool operator<=(const uuid &lhs, const uuid &rhs) noexcept;
XMIPP4_CONSTEXPR_CPP20 bool operator>(const uuid &lhs, const uuid &rhs) noexcept;
XMIPP4_CONSTEXPR_CPP20 bool operator>=(const uuid &lhs, const uuid &rhs) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, const uuid& id);

} // namespace xmipp4

#include "uuid.inl"
