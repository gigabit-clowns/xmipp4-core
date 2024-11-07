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

/**
 * @brief Location of a device as a (backend, id) tuple.
 * 
 */
class device_index
{
public:
    /**
     * @brief Construct a new device index from its components.
     * 
     * @tparam Str String type.
     * @param backend_name Name of the backend.
     * @param device_id Unique ID of the device within the backend.
     * 
     */
    template <typename Str>
    device_index(Str &&backend_name, std::size_t device_id);

    device_index() = default; 
    device_index(const device_index &other) = default; 
    device_index(device_index &&other) = default; 
    ~device_index() = default;

    device_index& operator=(const device_index &other) = default; 
    device_index& operator=(device_index &&other) = default; 

    /**
     * @brief Set the name of the device backend
     * 
     * @tparam Str String type,
     * @param backend_name Name of the backend.
     */
    template <typename Str>
    void set_backend_name(Str &&backend_name);

    /**
     * @brief Get the name of the device backend.
     * 
     * @return const std::string& The name.
     */
    const std::string& get_backend_name() const noexcept;

    /**
     * @brief Set the ID of the device within the backend.
     * 
     * @param device_id The device ID.
     */
    void set_device_id(std::size_t device_id) noexcept;

    /**
     * @brief Get the ID of the device within the backend.
     * 
     * @return std::size_t The device ID.
     */
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

/**
 * @brief Parse the device ID from a string.
 * 
 * A device ID string representation is expected to be:
 * <device_backend>:<device_id>
 * <device_backend> (assumes device_id=0)
 * 
 * @param path Appropriately formatted string with the device ID
 * @param result Output device_index object.
 * @return true The string was parsed successfully and the result was written.
 * @return false The string was not parsed and the result was not written.
 */
bool parse_device_index(std::string_view path, device_index &result);

} // namespace compute
} // namespace xmipp4

#include "device_index.inl"
