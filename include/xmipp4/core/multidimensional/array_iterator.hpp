// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

class array_iterator
{
public:
	XMIPP4_CORE_API
	array_iterator() noexcept;

	XMIPP4_CORE_API
	array_iterator(
		std::size_t ndim,
		std::vector<std::size_t> offsets
	);

	XMIPP4_CORE_API
	array_iterator(const array_iterator &other);
	XMIPP4_CORE_API
	array_iterator(array_iterator &&other) noexcept;
	XMIPP4_CORE_API
	~array_iterator();

	XMIPP4_CORE_API
	array_iterator& operator=(const array_iterator &other);
	XMIPP4_CORE_API
	array_iterator& operator=(array_iterator &&other) noexcept;

	XMIPP4_CORE_API
	span<const std::size_t> get_indices() const noexcept;

	XMIPP4_CORE_API
	span<std::size_t> get_indices() noexcept;

	XMIPP4_CORE_API
	span<const std::size_t> get_offsets() const noexcept;

	XMIPP4_CORE_API
	span<std::size_t> get_offsets() noexcept;

private:
	std::vector<std::size_t> m_indices;
	std::vector<std::size_t> m_offsets;
};

} // namespace multidimensional
} // namespace xmipp4
