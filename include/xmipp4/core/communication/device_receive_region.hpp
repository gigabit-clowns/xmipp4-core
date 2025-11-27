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
 * @brief Representation of a dynamically typed device memory region in a
 * device buffer.
 * 
 */
class device_receive_region
{
public:
	/**
	 * @brief Construct an empty region.
	 *
	 */
	device_receive_region() noexcept;

	device_receive_region(
		std::shared_ptr<hardware::buffer> buffer,
		numerical_type data_type, 
		std::size_t offset,
		std::size_t count
	) noexcept;

	device_receive_region(const device_receive_region &other) = default;
	device_receive_region(device_receive_region &&other) = default;
	~device_receive_region() = default;

	device_receive_region& operator=(const device_receive_region &other) = default;
	device_receive_region& operator=(device_receive_region &&other) = default;

	/**
	 * @brief Get the buffer
	 * 
	 * @return const std::shared_ptr<hardware::buffer>& The buffer.
	 */
	const std::shared_ptr<hardware::buffer>& get_buffer() const noexcept;

	/**
	 * @brief Get the type of the elements.
	 * 
	 * @return numerical_type The type.
	 */
	numerical_type get_data_type() const noexcept;

	/**
	 * @brief Get the offset into the buffer.
	 * 
	 * @return std::size_t The offset.
	 */
	std::size_t get_offset() const noexcept;

	/**
	 * @brief Get the number of elements.
	 * 
	 * @return std::size_t The element count.
	 */
	std::size_t get_count() const noexcept;

private:
	std::shared_ptr<hardware::buffer> m_buffer;
	numerical_type m_data_type;
	std::size_t m_offset;
	std::size_t m_count;

};

} // namespace communication
} // namespace xmipp4

#include "device_receive_region.inl"
