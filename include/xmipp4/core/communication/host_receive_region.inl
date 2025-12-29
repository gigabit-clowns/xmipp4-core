// SPDX-License-Identifier: GPL-3.0-only

#include "host_receive_region.hpp"

#include "../numerical_type_traits.hpp"

namespace xmipp4 
{
namespace communication
{

XMIPP4_INLINE_CONSTEXPR
host_receive_region::host_receive_region() noexcept
	: m_data(nullptr)
	, m_data_type(numerical_type::unknown)
	, m_count(0UL)
{
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR
host_receive_region::host_receive_region(T* data, std::size_t count) noexcept
	: m_data(data)
	, m_data_type(numerical_type_of<T>::value)
	, m_count(count)
{
}

XMIPP4_INLINE_CONSTEXPR
host_receive_region::host_receive_region(
	void *data, 
	numerical_type data_type, 
	std::size_t count
) noexcept
	: m_data(data)
	, m_data_type(data_type)
	, m_count(count)
{
}

XMIPP4_INLINE_CONSTEXPR 
void* host_receive_region::get_data() const noexcept
{
	return m_data;
}

XMIPP4_INLINE_CONSTEXPR 
numerical_type host_receive_region::get_data_type() const noexcept
{
	return m_data_type;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t host_receive_region::get_count() const noexcept
{
	return m_count;
}

} // namespace communication
} // namespace xmipp4
