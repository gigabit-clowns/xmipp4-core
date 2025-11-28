// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"

#include <memory>
#include <cstddef>

namespace xmipp4 
{

namespace hardware
{

class buffer;

} // hardware

namespace communication
{

/**
 * @brief Representation of a dynamically typed read-only device memory region
 * in a device buffer.
 * 
 */
class device_send_region
{
public:
	/**
	 * @brief Construct an empty region.
	 *
	 */
	device_send_region() noexcept;

	/**
	 * @brief Construct a new device send region from a const buffer.
	 * 
	 * @param buffer The buffer to be sent.
	 * @param data_type The data type of the elements in the buffer.
	 * @param offset Offset into the buffer. In elements.
	 * @param count Number of elements to be sent.
	 */
	device_send_region(
		std::shared_ptr<const hardware::buffer> buffer,
		numerical_type data_type, 
		std::size_t offset,
		std::size_t count
	) noexcept;

	device_send_region(const device_send_region &other) = default;
	device_send_region(device_send_region &&other) = default;
	~device_send_region() = default;

	device_send_region& operator=(const device_send_region &other) = default;
	device_send_region& operator=(device_send_region &&other) = default;

	/**
	 * @brief Get the buffer
	 * 
	 * @return const std::shared_ptr<hardware::buffer>& The buffer.
	 */
	const std::shared_ptr<const hardware::buffer>& get_data() const noexcept;

	/**
	 * @brief Get the type of the elements.
	 * 
	 * @return numerical_type The type.
	 */
	numerical_type get_data_type() const noexcept;

	/**
	 * @brief Get the offset into the buffer.
	 * 
	 * @return std::size_t The offset in elements.
	 */
	std::size_t get_offset() const noexcept;

	/**
	 * @brief Get the number of elements.
	 * 
	 * @return std::size_t The element count.
	 */
	std::size_t get_count() const noexcept;

private:
	std::shared_ptr<const hardware::buffer> m_buffer;
	numerical_type m_data_type;
	std::size_t m_offset;
	std::size_t m_count;
};

} // namespace communication
} // namespace xmipp4

#include "device_send_region.inl"
