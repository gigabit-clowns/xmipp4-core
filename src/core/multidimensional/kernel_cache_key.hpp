// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operation_id.hpp>

#include <boost/container/small_vector.hpp>

#include <functional>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_cache_key
{
public:
	kernel_cache_key(const kernel_cache_key &other) = default;
	kernel_cache_key(kernel_cache_key&& other) noexcept = default;
	~kernel_cache_key() = default;

	kernel_cache_key& operator=(const kernel_cache_key &other) = default;
	kernel_cache_key& operator=(kernel_cache_key&& other) noexcept = default;

	bool operator==(const kernel_cache_key &other) const noexcept;
	bool operator!=(const kernel_cache_key &other) const noexcept;

	std::size_t hash() const noexcept;

private:
	operation_id m_operation_id;
	std::string m_operation_parameters;
	boost::container::small_vector<array_descriptor, 24> m_operands;
	// TODO represent device. 
};

} // namespace multidimensional
} // namespace xmipp4

namespace std
{

template <>
struct hash<xmipp4::multidimensional::kernel_cache_key>
{
	size_t operator()(
		const xmipp4::multidimensional::kernel_cache_key& key
	) const noexcept;
};

} // namespace std

#include "kernel_cache_key.inl"
