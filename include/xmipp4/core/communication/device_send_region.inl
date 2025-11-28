// SPDX-License-Identifier: GPL-3.0-only

#include "device_send_region.hpp"

namespace xmipp4 
{
namespace communication
{

device_send_region::device_send_region() noexcept
	: m_data_type(numerical_type::unknown)
	, m_offset(0UL)
	, m_count(0UL)
{
}

device_send_region::device_send_region(
	std::shared_ptr<const hardware::buffer> buffer,
	numerical_type data_type, 
	std::size_t offset,
	std::size_t count
) noexcept
	: m_buffer(std::move(buffer))
	, m_data_type(data_type)
	, m_offset(offset)
	, m_count(count)
{
}

const std::shared_ptr<const hardware::buffer>& 
device_send_region::get_buffer() const noexcept
{
	return m_buffer;
}

numerical_type device_send_region::get_data_type() const noexcept
{
	return m_data_type;
}

std::size_t device_send_region::get_offset() const noexcept
{
	return m_offset;
}

std::size_t device_send_region::get_count() const noexcept
{
	return m_count;
}

} // namespace communication
} // namespace xmipp4
