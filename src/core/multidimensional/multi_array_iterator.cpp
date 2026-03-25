// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/multi_array_iterator.hpp>

namespace xmipp4 
{
namespace multidimensional
{

multi_array_iterator::multi_array_iterator() noexcept = default;
multi_array_iterator::multi_array_iterator(
	std::size_t ndim,
	std::vector<std::ptrdiff_t> offsets
)
	: m_indices(ndim, 0UL)
	, m_offsets(std::move(offsets))
{
}

multi_array_iterator::multi_array_iterator(
	const multi_array_iterator &other
) = default;
multi_array_iterator::multi_array_iterator(
	multi_array_iterator &&other
) noexcept = default;
multi_array_iterator::~multi_array_iterator() = default;

multi_array_iterator& 
multi_array_iterator::operator=(const multi_array_iterator &other) = default;
multi_array_iterator& 
multi_array_iterator::operator=(multi_array_iterator &&other) noexcept = default;

span<const std::size_t> multi_array_iterator::get_indices() const noexcept
{
	return make_span(m_indices);
}

span<std::size_t> multi_array_iterator::get_indices() noexcept
{
	return make_span(m_indices);
}

span<const std::ptrdiff_t> multi_array_iterator::get_offsets() const noexcept
{
	return make_span(m_offsets);
}

span<std::ptrdiff_t> multi_array_iterator::get_offsets() noexcept
{
	return make_span(m_offsets);
}

} // namespace multidimensional
} // namespace xmipp4
