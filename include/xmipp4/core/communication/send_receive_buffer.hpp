// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace communication
{

class send_receive_buffer
{
public:
	XMIPP4_CONSTEXPR
	send_receive_buffer() noexcept;
	XMIPP4_CONSTEXPR
	send_receive_buffer(
		const void *send_data, 
		void *receive_data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;
	XMIPP4_CONSTEXPR
	send_receive_buffer(
		void *send_recv_data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;
	send_receive_buffer(const send_receive_buffer &other) = default;
	send_receive_buffer(send_receive_buffer &&other) = default;
	~send_receive_buffer() = default;

	send_receive_buffer& operator=(const send_receive_buffer &other) = default;
	send_receive_buffer& operator=(send_receive_buffer &&other) = default;

	XMIPP4_CONSTEXPR const void* get_send_data() const noexcept;

	XMIPP4_CONSTEXPR void* get_receive_data() const noexcept;

	XMIPP4_CONSTEXPR numerical_type get_data_type() const noexcept;

	XMIPP4_CONSTEXPR std::size_t get_count() const noexcept;

private:
	const void *m_send_data;
	const void *m_receive_data;
	numerical_type m_data_type;
	std::size_t m_count;

};

} // namespace communication
} // namespace xmipp4

