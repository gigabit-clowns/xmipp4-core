// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/layout/access_layout.hpp>

#include "access_layout_implementation.hpp"

#include <sstream>

namespace xmipp4 
{
namespace layout
{

access_layout::access_layout() = default;

access_layout::access_layout(
	std::unique_ptr<const access_layout_implementation> implementation
)
	: m_implementation(std::move(implementation))
{
}

access_layout::access_layout(
	access_layout&& other
) noexcept = default;

access_layout::~access_layout() = default;

access_layout& 
access_layout::operator=(access_layout&& other) noexcept = default;

std::size_t access_layout::get_rank() const noexcept
{
	if (!m_implementation)
	{
		return 0UL;
	}

	return m_implementation->get_rank();
}

std::size_t access_layout::get_number_of_operands() const noexcept
{
	if (!m_implementation)
	{
		return 0UL;
	}
	
	return m_implementation->get_number_of_operands();
}

span<const std::size_t> access_layout::get_extents() const
{
	if (!m_implementation)
	{
		return span<const std::size_t>();
	}
	
	return m_implementation->get_extents();
}

span<const std::ptrdiff_t> 
access_layout::get_strides(std::size_t operand) const
{
	if (!m_implementation)
	{
		const access_layout_implementation implementation;
		return implementation.get_strides(operand); // throws
	}

	return m_implementation->get_strides(operand);
}

std::ptrdiff_t access_layout::get_offset(std::size_t operand) const
{
	if (!m_implementation)
	{
		const access_layout_implementation implementation;
		return implementation.get_offset(operand); // throws
	}

	return m_implementation->get_offset(operand);
}

std::size_t access_layout::iter(
	access_iterator &ite, 
	std::size_t first_dim,
	std::size_t last_dim
) const
{
	if (!m_implementation)
	{
		return 0UL;
	}

	return m_implementation->iter(ite, first_dim, last_dim);
}

std::size_t access_layout::next(
	access_iterator &ite, 
	std::size_t n,
	std::size_t first_dim,
	std::size_t last_dim
) const noexcept
{
	if (!m_implementation)
	{
		return 0UL;
	}

	return m_implementation->next(ite, n, first_dim, last_dim);
}

const access_layout_implementation*
access_layout::get_implementation() const noexcept
{
	return m_implementation.get();
}

} // namespace layout
} // namespace xmipp4
