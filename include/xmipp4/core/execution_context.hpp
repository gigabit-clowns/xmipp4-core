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
class device_queue;
class memory_allocator;

} // namespace hardware

namespace multidimensional
{

class operation_dispatcher;

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
	 * @brief Get the device handle for this context.
	 * 
	 * @return hardware::device& The device handle.
	 */
	XMIPP4_CORE_API
	hardware::device& get_device() const;

	/**
	 * @brief Get the memory allocator responsible to allocate memory in the
	 * requested placement.
	 * 
	 * @param placement Region where memory should be placed.
	 * @return memory_allocator& The memory allocator.
	 * 
	 * @note Depending on the memory architecture, some allocators may alias
	 * each other.
	 */
	XMIPP4_CORE_API
	hardware::memory_allocator& 
	get_memory_allocator(hardware::target_placement placement) const;

	/**
	 * @brief Get the optimal data alignment for the device.
	 * 
	 * @return std::size_t The optimal data alignment in bytes.
	 */
	XMIPP4_CORE_API
	std::size_t get_optimal_data_alignment() const;

	/**
	 * @brief Set the active queue.
	 * 
	 * @param queue The queue to be set as active.
	 * @return std::shared_ptr<device_queue> The previous active queue.
	 */
	XMIPP4_CORE_API
	std::shared_ptr<hardware::device_queue> 
	set_active_queue(std::shared_ptr<hardware::device_queue> queue);

	/**
	 * @brief Get the active queue.
	 * 
	 * @return const std::shared_ptr<device_queue>& The active queue.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<hardware::device_queue>& get_active_queue() const;

	/**
	 * @brief Get the operation dispatcher.
	 * 
	 * @return multidimensional::operation_dispatcher& The dispatcher.
	 */
	multidimensional::operation_dispatcher& 
	get_operation_dispatcher() const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	implementation& get_implementation();
	const implementation& get_implementation() const;
};

} // namespace xmipp4
