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
 * @file device_index.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the compute::device_index class
 * @date 2024-10-23
 * 
 */

#include "../platform/constexpr.hpp"

#include <string>
#include <ostream>

namespace xmipp4 
{
namespace compute
{

class device_index
{
public:
    template <typename Str>
    device_index(Str &&backend_name, std::size_t device_id);
    device_index() = default; 
    device_index(const device_index &other) = default; 
    device_index(device_index &&other) = default; 
    ~device_index() = default;

    device_index& operator=(const device_index &other) = default; 
    device_index& operator=(device_index &&other) = default; 

    template <typename Str>
    void set_backend_name(Str &&backend_name);

    const std::string& get_backend_name() const noexcept;

    void set_device_id(std::size_t device_id) noexcept;

    std::size_t get_device_id() const noexcept;

private:
    std::string m_backend_name;
    std::size_t m_device_id;

};



bool operator==(const device_index &lhs, const device_index &rhs) noexcept;
bool operator!=(const device_index &lhs, const device_index &rhs) noexcept;
bool operator<(const device_index &lhs, const device_index &rhs) noexcept;
bool operator<=(const device_index &lhs, const device_index &rhs) noexcept;
bool operator>(const device_index &lhs, const device_index &rhs) noexcept;
bool operator>=(const device_index &lhs, const device_index &rhs) noexcept;

template <typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, const device_index &index);

bool parse_device_index(const std::string &index, device_index &result);

} // namespace compute
} // namespace xmipp4

#include "device_index.inl"
