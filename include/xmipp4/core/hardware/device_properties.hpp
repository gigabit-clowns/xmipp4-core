// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_type.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <string>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief Description of the characteristics of a physical device
 * in the host.
 * 
 */
class device_properties
{
public:
	XMIPP4_CORE_API
	device_properties() noexcept;
	device_properties(const device_properties& other);
	XMIPP4_CORE_API
	device_properties(device_properties&& other) noexcept;
	XMIPP4_CORE_API
	~device_properties();

	device_properties& operator=(const device_properties& other);
	XMIPP4_CORE_API
	device_properties& operator=(device_properties&& other) noexcept;

	/**
	 * @brief Set the device type.
	 * 
	 * @param type The device type.
	 */
	XMIPP4_CORE_API
	void set_type(device_type type);

	/**
	 * @brief Get the device type.
	 * 
	 * @return device_type The device type. 
	 */
	XMIPP4_CORE_API
	device_type get_type() const noexcept;

	/**
	 * @brief Set the device name.
	 * 
	 * @tparam Str String type.
	 * @param name The device name.
	 */
	XMIPP4_CORE_API
	void set_name(const std::string &name);

	/**
	 * @brief Get the device name.
	 * 
	 * @return const std::string& The device name.
	 */
	XMIPP4_CORE_API
	const std::string& get_name() const noexcept;

	/**
	 * @brief Set the physical location of the device.
	 * 
	 * Usually this is a PCI address.
	 * 
	 * @tparam Str String type.
	 * @param location The physical location of the device.
	 */
	XMIPP4_CORE_API
	void set_physical_location(const std::string &location);

	/**
	 * @brief Get the physical location of the device.
	 * 
	 * Usually this is a PCI address.
	 * 
	 * @return const std::string& The physical location of the device.
	 */
	XMIPP4_CORE_API
	const std::string& get_physical_location() const noexcept;

	/**
	 * @brief Set the total memory of the device in bytes.
	 * 
	 * @param bytes Number of bytes.
	 */
	XMIPP4_CORE_API
	void set_total_memory_bytes(std::size_t bytes);

	/**
	 * @brief Get the total memory of the device in bytes.
	 * 
	 * @return std::size_t Number of bytes
	 */
	XMIPP4_CORE_API
	std::size_t get_total_memory_bytes() const noexcept;

	/**
	 * @brief Set the optimal data alignment.
	 * 
	 * @param bytes The optimal data alignment in bytes.
	 */
	XMIPP4_CORE_API
	void set_optimal_data_alignment(std::size_t alignment);

	/**
	 * @brief Get the optimal data alignment.
	 * 
	 * @return std::size_t The optimal data alignment in bytes.
	 */
	XMIPP4_CORE_API
	std::size_t get_optimal_data_alignment() const noexcept;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	implementation& create_implementation_if_null();
	const implementation& get_implementation() const noexcept;
};

} // namespace hardware
} // namespace xmipp4
