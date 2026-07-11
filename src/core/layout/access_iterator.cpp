// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/layout/access_iterator.hpp>

namespace xmipp4 
{

access_iterator::access_iterator() noexcept = default;
access_iterator::access_iterator(
	std::size_t ndim,
	std::vector<std::ptrdiff_t> offsets
)
	: m_indices(ndim, 0UL)
	, m_offsets(std::move(offsets))
{
}

access_iterator::access_iterator(
	const access_iterator &other
) = default;
access_iterator::access_iterator(
	access_iterator &&other
) noexcept = default;
access_iterator::~access_iterator() = default;

access_iterator& 
access_iterator::operator=(const access_iterator &other) = default;
access_iterator& 
access_iterator::operator=(access_iterator &&other) noexcept = default;

span<const std::size_t> access_iterator::get_indices() const noexcept
{
	return make_span(m_indices);
}

span<std::size_t> access_iterator::get_indices() noexcept
{
	return make_span(m_indices);
}

span<const std::ptrdiff_t> access_iterator::get_offsets() const noexcept
{
	return make_span(m_offsets);
}

span<std::ptrdiff_t> access_iterator::get_offsets() noexcept
{
	return make_span(m_offsets);
}

} // namespace xmipp4
