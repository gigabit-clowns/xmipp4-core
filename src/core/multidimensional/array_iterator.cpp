// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_iterator.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array_iterator::array_iterator() noexcept = default;
array_iterator::array_iterator(
	std::size_t ndim,
	std::vector<std::ptrdiff_t> offsets
)
	: m_indices(ndim, 0UL)
	, m_offsets(std::move(offsets))
{
}

array_iterator::array_iterator(const array_iterator &other) = default;
array_iterator::array_iterator(array_iterator &&other) noexcept = default;
array_iterator::~array_iterator() = default;

array_iterator& 
array_iterator::operator=(const array_iterator &other) = default;
array_iterator& 
array_iterator::operator=(array_iterator &&other) noexcept = default;

span<const std::size_t> array_iterator::get_indices() const noexcept
{
	return make_span(m_indices);
}

span<std::size_t> array_iterator::get_indices() noexcept
{
	return make_span(m_indices);
}

span<const std::ptrdiff_t> array_iterator::get_offsets() const noexcept
{
	return make_span(m_offsets);
}

span<std::ptrdiff_t> array_iterator::get_offsets() noexcept
{
	return make_span(m_offsets);
}

} // namespace multidimensional
} // namespace xmipp4
