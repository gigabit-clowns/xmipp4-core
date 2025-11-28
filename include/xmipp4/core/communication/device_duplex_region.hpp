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
 * @brief Represents an equally type and sized pair of a dynamically typed 
 * memory regions in device buffers.
 * 
 */
class device_duplex_region
{
public:
	/**
	 * @brief Construct an empty region pair.
	 *
	 */
	device_duplex_region() noexcept;

	/**
	 * @brief Construct a new device duplex region from send and receive 
	 * buffers.
	 * 
	 * @param send_buffer The buffer to be sent.
	 * @param receive_buffer The buffer where elements are received.
	 * @param data_type Data type of the elements.
	 * @param send_offset Offset into the send_buffer. In elements.
	 * @param receive_offset Offset into the receive_buffer. In elements.
	 * @param count Number of elements to be sent and received.
	 */
	device_duplex_region(
		std::shared_ptr<const hardware::buffer> send_buffer,
		std::shared_ptr<hardware::buffer> receive_buffer,
		numerical_type data_type, 
		std::size_t send_offset,
		std::size_t receive_offset,
		std::size_t count
	) noexcept;

	/**
	 * @brief Construct a new device duplex region with aliasing buffers.
	 * 
	 * @param buffer The buffer to be sent and received.
	 * @param data_type Data type of the elements.
	 * @param offset Offset into the send_buffer. In elements.
	 * @param count Number of elements to be sent and received.
	 */
	device_duplex_region(
		std::shared_ptr<hardware::buffer> buffer,
		numerical_type data_type, 
		std::size_t offset,
		std::size_t count
	) noexcept;

	device_duplex_region(const device_duplex_region &other) = default;
	device_duplex_region(device_duplex_region &&other) = default;
	~device_duplex_region() = default;

	device_duplex_region& 
	operator=(const device_duplex_region &other) = default;
	device_duplex_region& 
	operator=(device_duplex_region &&other) = default;

	/**
	 * @brief Get the send buffer.
	 * 
	 * @return const std::shared_ptr<hardware::buffer>& The send buffer.
	 */
	const std::shared_ptr<const hardware::buffer>& 
	get_send_buffer() const noexcept;

	/**
	 * @brief Get the receive buffer.
	 * 
	 * @return const std::shared_ptr<hardware::buffer>& The receive buffer.
	 */
	const std::shared_ptr<hardware::buffer>& 
	get_receive_buffer() const noexcept;

	/**
	 * @brief Get the data type.
	 * 
	 * @return numerical_type The data type.
	 */
	numerical_type get_data_type() const noexcept;

	/**
	 * @brief Get the offset into the send buffer.
	 * 
	 * @return std::size_t The offset in elements.
	 */
	std::size_t get_send_offset() const noexcept;

	/**
	 * @brief Get the offset into the receive buffer.
	 * 
	 * @return std::size_t The offset in elements.
	 */
	std::size_t get_receive_offset() const noexcept;

	/**
	 * @brief Get the number of elements referenced by the send and receive
	 * regions.
	 * 
	 * @return std::size_t The element count.
	 */
	std::size_t get_count() const noexcept;

private:
	std::shared_ptr<const hardware::buffer> m_send_buffer;
	std::shared_ptr<hardware::buffer> m_receive_buffer;
	numerical_type m_data_type;
	std::size_t m_send_offset;
	std::size_t m_receive_offset;
	std::size_t m_count;

};

} // namespace communication
} // namespace xmipp4

#include "device_duplex_region.inl"
