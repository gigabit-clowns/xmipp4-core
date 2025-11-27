// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace communication
{

class host_receive_region
{
public:
	XMIPP4_CONSTEXPR
	host_receive_region() noexcept;

	template <typename T>
	XMIPP4_CONSTEXPR
	host_receive_region(T* data, std::size_t count) noexcept;

	XMIPP4_CONSTEXPR
	host_receive_region(
		void *data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;

	host_receive_region(const host_receive_region &other) = default;
	host_receive_region(host_receive_region &&other) = default;
	~host_receive_region() = default;

	host_receive_region& operator=(const host_receive_region &other) = default;
	host_receive_region& operator=(host_receive_region &&other) = default;

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

#include "host_receive_region.inl"
