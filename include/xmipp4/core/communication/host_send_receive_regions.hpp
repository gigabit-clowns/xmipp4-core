// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace communication
{

class host_send_receive_regions
{
public:
	XMIPP4_CONSTEXPR
	host_send_receive_regions() noexcept;

	template <typename T>
	XMIPP4_CONSTEXPR
	host_send_receive_regions(
		const T *send_data, 
		T *recv_data, 
		std::size_t count
	) noexcept;

	XMIPP4_CONSTEXPR
	host_send_receive_regions(
		const void *send_data, 
		void *receive_data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;

	template <typename T>
	XMIPP4_CONSTEXPR
	host_send_receive_regions(T *send_recv_data, std::size_t count) noexcept;

	XMIPP4_CONSTEXPR
	host_send_receive_regions(
		void *send_recv_data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;
	host_send_receive_regions(const host_send_receive_regions &other) = default;
	host_send_receive_regions(host_send_receive_regions &&other) = default;
	~host_send_receive_regions() = default;

	host_send_receive_regions& 
	operator=(const host_send_receive_regions &other) = default;
	host_send_receive_regions& 
	operator=(host_send_receive_regions &&other) = default;

	XMIPP4_CONSTEXPR const void* get_send_data() const noexcept;

	XMIPP4_CONSTEXPR void* get_receive_data() const noexcept;

	XMIPP4_CONSTEXPR numerical_type get_data_type() const noexcept;

	XMIPP4_CONSTEXPR std::size_t get_count() const noexcept;

private:
	const void *m_send_data;
	void *m_receive_data;
	numerical_type m_data_type;
	std::size_t m_count;

};

} // namespace communication
} // namespace xmipp4

#include "host_send_receive_regions.inl"
