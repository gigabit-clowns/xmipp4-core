// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/constexpr.hpp"

#include <string>
#include <string_view>
#include <ostream>

namespace xmipp4 
{
namespace hardware
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
	 * @param backend_name Name of the backend.
	 * @param device_id Unique ID of the device within the backend.
	 *
	 */
	device_index(const std::string &backend_name, std::size_t device_id);

	/**
	 * @brief Construct a new device index from its components.
	 *
	 * @param backend_name Name of the backend (moved).
	 * @param device_id Unique ID of the device within the backend.
	 *
	 */
	device_index(std::string &&backend_name, std::size_t device_id);

	device_index() = default; 
	device_index(const device_index &other) = default; 
	device_index(device_index &&other) = default; 
	~device_index() = default;

	device_index& operator=(const device_index &other) = default; 
	device_index& operator=(device_index &&other) = default; 

	/**
	 * @brief Get the name of the device backend.
	 * 
	 * @return const std::string& The name.
	 */
	const std::string& get_backend_name() const noexcept;

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

std::ostream& operator<<(std::ostream &os, const device_index &index);

/**
 * @brief Parse the device ID from a string.
 *
 * A device ID string representation is expected to be:
 * <device_backend>:<device_id>
 * <device_backend> (assumes device_id=0)
 *
 * @param text Appropriately formatted string with the device ID
 * @param result Output device_index object.
 * @return true The string was parsed successfully and the result was written.
 * @return false The string was not parsed and the result was not written.
 */
bool parse_device_index(std::string_view text, device_index &result);

} // namespace hardware
} // namespace xmipp4
