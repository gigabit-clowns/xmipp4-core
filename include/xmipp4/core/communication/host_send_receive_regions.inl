// SPDX-License-Identifier: GPL-3.0-only

#include "host_send_receive_regions.hpp"

#include "../numerical_type_of.hpp"

namespace xmipp4 
{
namespace communication
{

XMIPP4_INLINE_CONSTEXPR
host_send_receive_regions::host_send_receive_regions() noexcept
	: m_send_data(nullptr)
	, m_receive_data(nullptr)
	, m_data_type(numerical_type::unknown)
	, m_count(0UL)
{
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR
host_send_receive_regions::host_send_receive_regions(
	const T *send_data, 
	T *receive_data, 
	std::size_t count
) noexcept
	: m_send_data(send_data)
	, m_receive_data(receive_data)
	, m_data_type(numerical_type_of<T>::value())
	, m_count(count)
{
}

XMIPP4_INLINE_CONSTEXPR
host_send_receive_regions::host_send_receive_regions(
	const void *send_data, 
	void *receive_data, 
	numerical_type data_type, 
	std::size_t count
) noexcept
	: m_send_data(send_data)
	, m_receive_data(receive_data)
	, m_data_type(data_type)
	, m_count(count)
{
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR
host_send_receive_regions::host_send_receive_regions(
	T *send_recv_data, 
	std::size_t count
) noexcept
	: m_send_data(send_recv_data)
	, m_receive_data(send_recv_data)
	, m_data_type(numerical_type_of<T>::value())
	, m_count(count)
{
}

XMIPP4_INLINE_CONSTEXPR
host_send_receive_regions::host_send_receive_regions(
	void *send_recv_data, 
	numerical_type data_type, 
	std::size_t count
) noexcept
	: m_send_data(send_recv_data)
	, m_receive_data(send_recv_data)
	, m_data_type(data_type)
	, m_count(count)
{
}

XMIPP4_INLINE_CONSTEXPR 
const void* host_send_receive_regions::get_send_data() const noexcept
{
	return m_send_data;
}	

XMIPP4_INLINE_CONSTEXPR
void* host_send_receive_regions::get_receive_data() const noexcept
{
	return m_receive_data;
}

XMIPP4_INLINE_CONSTEXPR 
numerical_type host_send_receive_regions::get_data_type() const noexcept
{
	return m_data_type;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t host_send_receive_regions::get_count() const noexcept
{
	return m_count;
}

} // namespace communication
} // namespace xmipp4
