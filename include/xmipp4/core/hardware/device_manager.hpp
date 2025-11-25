// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_index.hpp"
#include "device_properties.hpp"
#include "device_backend.hpp"
#include "../named_service_manager.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace hardware
{

class device;

/**
 * @brief Centralize multiple device_backends.
 * 
 */
class device_manager final
	: public named_service_manager
{
public:
	XMIPP4_CORE_API device_manager();
	device_manager(const device_manager &other) = delete;
	device_manager(device_manager &&other) noexcept = delete;
	XMIPP4_CORE_API ~device_manager() override;

	device_manager& operator=(const device_manager &other) = delete;
	device_manager& operator=(device_manager &&other) = delete;

	XMIPP4_CORE_API
	void register_builtin_backends() override;

	XMIPP4_CORE_API
	void enumerate_backends(std::vector<std::string> &backends) const override;

	XMIPP4_CORE_API
	device_backend* get_backend(const std::string &name) const override;

	/**
	 * @brief Register a new backend.
	 * 
	 * @param backend The backend to be registered.
	 * @return true if the backend was successfully registered.
	 * @return false if the backend could not be registered.
	 * 
	 */
	XMIPP4_CORE_API
	bool register_backend(std::unique_ptr<device_backend> backend);

	/**
	 * @brief Enumerate devices across all backends.
	 * 
	 * @param indices Output parameter with the list of device indices.
	 * @note The output list is cleared before populating it.
	 * 
	 */
	XMIPP4_CORE_API
	void enumerate_devices(std::vector<device_index> &indices) const;

	/**
	 * @brief Query the properties of a device.
	 * 
	 * @param index The index of the device.
	 * @param properties Output parameter with device properties.
	 * @return true Device exists and properties were written.
	 * @return false Device does not exist and properties were not written.
	 */
	XMIPP4_CORE_API
	bool get_device_properties(
		const device_index &index, 
		device_properties &properties 
	) const;

	/**
	 * @brief Create a device handle.
	 * 
	 * @param index Index of the device.
	 * @param params Parameters used for device instantiation.
	 * @return std::shared_ptr<device> The device handle.
	 */
	XMIPP4_CORE_API 
	std::shared_ptr<device> create_device(const device_index &index) const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	void create_implementation_if_null();
}; 

} // namespace hardware
} // namespace xmipp4
