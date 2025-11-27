// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace communication
{

class send_buffer
{
public:
	XMIPP4_CONSTEXPR
	send_buffer() noexcept;
	XMIPP4_CONSTEXPR
	send_buffer(
		void *data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;
	send_buffer(const send_buffer &other) = default;
	send_buffer(send_buffer &&other) = default;
	~send_buffer() = default;

	send_buffer& operator=(const send_buffer &other) = default;
	send_buffer& operator=(send_buffer &&other) = default;

	XMIPP4_CONSTEXPR const void* get_data() const noexcept;

	XMIPP4_CONSTEXPR numerical_type get_data_type() const noexcept;

	XMIPP4_CONSTEXPR std::size_t get_count() const noexcept;

private:
	const void *m_data;
	numerical_type m_data_type;
	std::size_t m_count;

};

} // namespace communication
} // namespace xmipp4

