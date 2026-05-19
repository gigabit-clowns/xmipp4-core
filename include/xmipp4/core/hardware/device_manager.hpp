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
 * @brief Centralize multiple @ref device_backend instances and route
 * device queries to the appropriate one.
 *
 * The manager owns the registered backends and exposes a unified view of
 * all devices known to the framework, identified across backends by
 * @ref device_index. Backends are typically contributed by plugins at
 * load time via @ref register_backend, with @ref register_builtin_backends
 * adding those that are statically linked.
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
	void enumerate_backends(std::vector<std::string> &names) const override;

	XMIPP4_CORE_API
	device_backend* get_backend(const std::string &name) const override;

	/**
	 * @brief Register a new backend.
	 *
	 * Takes ownership of @p backend and keeps it alive for the rest of
	 * the manager's lifetime, indexed by the name it reports through
	 * @ref named_backend::get_name. Registration fails if another backend
	 * with the same name is already registered or if @p backend is null;
	 * on failure, ownership of @p backend remains with the caller (the
	 * unique_ptr is not consumed) and the manager state is unchanged.
	 *
	 * @param[in] backend The backend to be registered. Must be non-null
	 * for registration to succeed.
	 * @return true The backend was successfully registered and is now
	 * owned by the manager.
	 * @return false A name collision occurred or @p backend was null;
	 * the manager is left unchanged.
	 */
	XMIPP4_CORE_API
	bool register_backend(std::unique_ptr<device_backend> backend);

	/**
	 * @brief Enumerate devices across all registered backends.
	 *
	 * Asks every registered backend to enumerate its devices and
	 * aggregates the results into @p indices. The relative order of
	 * backends in the output is implementation defined; within a single
	 * backend the order matches whatever
	 * @ref device_backend::enumerate_devices reports.
	 *
	 * @param[out] indices Output list of device indices. Cleared before
	 * being populated, so existing contents are discarded.
	 */
	XMIPP4_CORE_API
	void enumerate_devices(std::vector<device_index> &indices) const;

	/**
	 * @brief Query the properties of a device.
	 *
	 * Looks up the backend named by @p index and forwards the query to
	 * @ref device_backend::get_device_properties. Fails if the backend
	 * is not registered or if the backend itself does not recognize the
	 * device id.
	 *
	 * @param[in] index Index identifying the target device.
	 * @param[out] properties Output device properties. Only modified
	 * when the call succeeds.
	 * @return true The device exists and @p properties has been written.
	 * @return false The backend or device referenced by @p index does
	 * not exist; @p properties is left untouched.
	 */
	XMIPP4_CORE_API
	bool get_device_properties(
		const device_index &index,
		device_properties &properties
	) const;

	/**
	 * @brief Create a device handle.
	 *
	 * Resolves the backend named by @p index and delegates the actual
	 * instantiation to @ref device_backend::create_device.
	 *
	 * @param[in] index Index identifying the target device.
	 * @return std::shared_ptr<device> A non-null handle to the device.
	 * @throws std::invalid_argument If no backend is registered under
	 * the name carried by @p index. The backend may additionally throw
	 * if the device id is invalid.
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
