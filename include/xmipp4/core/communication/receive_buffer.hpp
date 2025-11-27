// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace communication
{

class receive_buffer
{
public:
	XMIPP4_CONSTEXPR
	receive_buffer() noexcept;
	XMIPP4_CONSTEXPR
	receive_buffer(
		void *data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;
	receive_buffer(const receive_buffer &other) = default;
	receive_buffer(receive_buffer &&other) = default;
	~receive_buffer() = default;

	receive_buffer& operator=(const receive_buffer &other) = default;
	receive_buffer& operator=(receive_buffer &&other) = default;

	XMIPP4_CONSTEXPR void* get_data() const noexcept;

	XMIPP4_CONSTEXPR numerical_type get_data_type() const noexcept;

	XMIPP4_CONSTEXPR std::size_t get_count() const noexcept;

private:
	void *m_data;
	numerical_type m_data_type;
	std::size_t m_count;

};

} // namespace communication
} // namespace xmipp4

