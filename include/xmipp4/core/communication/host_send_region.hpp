// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace communication
{

class host_send_region
{
public:
	XMIPP4_CONSTEXPR
	host_send_region() noexcept;

	template <typename T>
	XMIPP4_CONSTEXPR
	host_send_region(const T *data, std::size_t count) noexcept;

	XMIPP4_CONSTEXPR
	host_send_region(
		const void *data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;

	host_send_region(const host_send_region &other) = default;
	host_send_region(host_send_region &&other) = default;
	~host_send_region() = default;

	host_send_region& operator=(const host_send_region &other) = default;
	host_send_region& operator=(host_send_region &&other) = default;

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

#include "host_send_region.inl"
