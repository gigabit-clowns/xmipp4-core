// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/multi_array_access_layout.hpp>

#include "multi_array_access_layout_implementation.hpp"

#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

multi_array_access_layout::multi_array_access_layout() = default;

multi_array_access_layout::multi_array_access_layout(
	std::unique_ptr<const multi_array_access_layout_implementation> implementation
)
	: m_implementation(std::move(implementation))
{
}

multi_array_access_layout::multi_array_access_layout(
	multi_array_access_layout&& other
) noexcept = default;

multi_array_access_layout::~multi_array_access_layout() = default;

multi_array_access_layout& 
multi_array_access_layout::operator=(multi_array_access_layout&& other) noexcept = default;

std::size_t multi_array_access_layout::get_rank() const noexcept
{
	if (!m_implementation)
	{
		return 0UL;
	}

	return m_implementation->get_rank();
}

std::size_t multi_array_access_layout::get_number_of_operands() const noexcept
{
	if (!m_implementation)
	{
		return 0UL;
	}
	
	return m_implementation->get_number_of_operands();
}

span<const std::size_t> multi_array_access_layout::get_extents() const
{
	if (!m_implementation)
	{
		return span<const std::size_t>();
	}
	
	return m_implementation->get_extents();
}

span<const std::ptrdiff_t> 
multi_array_access_layout::get_strides(std::size_t operand) const
{
	if (!m_implementation)
	{
		const multi_array_access_layout_implementation implementation;
		return implementation.get_strides(operand); // throws
	}

	return m_implementation->get_strides(operand);
}

std::ptrdiff_t multi_array_access_layout::get_offset(std::size_t operand) const
{
	if (!m_implementation)
	{
		const multi_array_access_layout_implementation implementation;
		return implementation.get_offset(operand); // throws
	}

	return m_implementation->get_offset(operand);
}

std::size_t multi_array_access_layout::iter(
	array_iterator &ite, 
	std::size_t dim
) const
{
	if (!m_implementation)
	{
		return 0UL;
	}

	return m_implementation->iter(ite, dim);
}

std::size_t multi_array_access_layout::next(
	array_iterator &ite, 
	std::size_t n,
	std::size_t dim
) const noexcept
{
	if (!m_implementation)
	{
		return 0UL;
	}

	return m_implementation->next(ite, n, dim);
}

const multi_array_access_layout_implementation*
multi_array_access_layout::get_implementation() const noexcept
{
	return m_implementation.get();
}

} // namespace multidimensional
} // namespace xmipp4
