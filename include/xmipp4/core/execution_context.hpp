// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{

class service_catalog;

namespace hardware
{

class device;
class device_index;
class device_properties;
class command_queue;
class memory_allocator;

} // namespace hardware

namespace multidimensional
{

class operation_command_manager;

} // namespace multidimensional

/**
 * @brief Centralization and management of execution related resources.
 */
class execution_context
{
public:
	/**
	 * @brief Default construct a device context object.
	 * 
	 * The resulting object is in a valid state but no methods may be called
	 * on it. Only useful as a placeholder.
	 */
	XMIPP4_CORE_API
	execution_context() noexcept;

	/**
	 * @brief Construct a device context object.
	 * 
	 * @param catalog The service catalog used to locate required services.
	 * @param index The device index for which the a device will be created.
	 */
	XMIPP4_CORE_API
	execution_context(
		service_catalog &catalog, 
		const hardware::device_index &index
	);

	execution_context(const execution_context &other) = delete;
	XMIPP4_CORE_API
	execution_context(execution_context &&other) noexcept;
	XMIPP4_CORE_API
	~execution_context();

	execution_context& operator=(const execution_context &other) = delete;
	XMIPP4_CORE_API
	execution_context& operator=(execution_context &&other) noexcept;

	/**
	 * @brief Get the properties of the device from this context.
	 * 
	 * @return const hardware::device_properties& The device properties.
	 */
	XMIPP4_CORE_API
	const hardware::device_properties& get_device_properties() const;

	/**
	 * @brief Get the device handle for this context.
	 * 
	 * @return hardware::device& The device handle.
	 */
	XMIPP4_CORE_API
	hardware::device& get_device() const;

	/**
	 * @brief Set the active queue.
	 *
	 * @param queue The queue to be set as active.
	 * @return std::shared_ptr<command_queue> The previous active queue.
	 */
	XMIPP4_CORE_API
	std::shared_ptr<hardware::command_queue>
	set_active_queue(std::shared_ptr<hardware::command_queue> queue);

	/**
	 * @brief Get the active queue.
	 *
	 * @return const std::shared_ptr<command_queue>& The active queue.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<hardware::command_queue>& get_active_queue() const;

	/**
	 * @brief Set the active memory allocator.
	 *
	 * @param allocator The allocator to be set as active.
	 * @return std::shared_ptr<memory_allocator> The previous active allocator.
	 */
	XMIPP4_CORE_API
	std::shared_ptr<hardware::memory_allocator>
	set_active_allocator(std::shared_ptr<hardware::memory_allocator> allocator);

	/**
	 * @brief Get the active allocator.
	 *
	 * @return const std::shared_ptr<device_allocator>& The active allocator.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<hardware::memory_allocator>& 
	get_active_allocator() const;

	/**
	 * @brief Get a reference to the operation command manager.
	 *
	 * @return const operation_command_manager& The operation command manager.
	 */
	XMIPP4_CORE_API
	const multidimensional::operation_command_manager& 
	get_operation_command_manager() const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	implementation& get_implementation();
	const implementation& get_implementation() const;
};

} // namespace xmipp4
