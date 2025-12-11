// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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

class device_context
{
public:
	/**
	 * @brief Default construct a device context object.
	 * 
	 * The resulting object is in a valid state but no methods may be called
	 * on it. Only useful as a placeholder.
	 * 
	 */
	device_context() noexcept;

	/**
	 * @brief Construct a device context object.
	 * 
	 * @param catalog The service catalog used to locate required services.
	 * @param index The device index for which the a device will be created.
	 */
	device_context(service_catalog &catalog, const device_index &index);

	device_context(const device_context &other) = delete;
	device_context(device_context &&other) noexcept;
	~device_context();

	device_context& operator=(const device_context &other) = delete;
	device_context& operator=(device_context &&other) noexcept;

	/**
	 * @brief Get the device handle for this context.
	 * 
	 * @return device& The device handle.
	 */
	device& get_device() const;

	/**
	 * @brief Get the memory allocator responsible to allocate memory within
	 * the device.
	 * 
	 * @return memory_allocator& The device local memory allocator.
	 */
	memory_allocator& get_device_local_memory_allocator() const;

	/**
	 * @brief Get the memory allocator responsible to allocate memory that is
	 * accessible from the host.
	 * 
	 * @return memory_allocator& The host accessible memory allocator.
	 * 
	 * @note In unified memory architectures, this allocator may be an alias
	 * of the device memory allocator.
	 */
	memory_allocator& get_host_accessible_memory_allocator() const;

	/**
	 * @brief Set the active queue.
	 * 
	 * @param queue The queue to be set as active.
	 * @return std::shared_ptr<device_queue> The previous active queue.
	 */
	std::shared_ptr<device_queue> 
	set_active_queue(std::shared_ptr<device_queue> queue);

	/**
	 * @brief Get the active queue.
	 * 
	 * @return const std::shared_ptr<device_queue>& The active queue.
	 */
	const std::shared_ptr<device_queue>& get_active_queue() const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	implementation& get_implementation();
	const implementation& get_implementation() const;

};

} // namespace hardware
} // namespace xmipp4
