// SPDX-License-Identifier: GPL-3.0-only

#include "device_duplex_region.hpp"

namespace xmipp4 
{
namespace communication
{

inline
device_duplex_region::device_duplex_region() noexcept
	: m_data_type(numerical_type::unknown)
	, m_send_offset(0)
	, m_receive_offset(0)
	, m_count(0)
{
}

inline
device_duplex_region::device_duplex_region(
	std::shared_ptr<const hardware::buffer> send_buffer,
	std::shared_ptr<hardware::buffer> receive_buffer,
	numerical_type data_type, 
	std::size_t send_offset,
	std::size_t receive_offset,
	std::size_t count
) noexcept
	: m_send_buffer(std::move(send_buffer))
	, m_receive_buffer(std::move(receive_buffer))
	, m_data_type(data_type)
	, m_send_offset(send_offset)
	, m_receive_offset(receive_offset)
	, m_count(count)
{
}

inline
device_duplex_region::device_duplex_region(
	std::shared_ptr<hardware::buffer> buffer,
	numerical_type data_type, 
	std::size_t offset,
	std::size_t count
) noexcept
	: m_send_buffer(buffer)
	, m_receive_buffer(std::move(buffer))
	, m_data_type(data_type)
	, m_send_offset(offset)
	, m_receive_offset(offset)
	, m_count(count)
{
}

inline
const std::shared_ptr<const hardware::buffer>& 
device_duplex_region::get_send_buffer() const noexcept
{
	return m_send_buffer;
}

inline
const std::shared_ptr<hardware::buffer>& 
device_duplex_region::get_receive_buffer() const noexcept
{
	return m_receive_buffer;
}

inline
numerical_type device_duplex_region::get_data_type() const noexcept
{
	return m_data_type;
}

inline
std::size_t device_duplex_region::get_send_offset() const noexcept
{
	return m_send_offset;
}

inline
std::size_t device_duplex_region::get_receive_offset() const noexcept
{
	return m_receive_offset;
}

inline
std::size_t device_duplex_region::get_count() const noexcept
{
	return m_count;
}

} // namespace communication
} // namespace xmipp4
