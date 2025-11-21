// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "array_access_layout_implementation.hpp"
#include "strided_layout_implementation.hpp"

namespace xmipp4 
{
namespace multidimensional
{

array_access_layout_builder::array_access_layout_builder() noexcept = default;

array_access_layout_builder::array_access_layout_builder(
	array_access_layout_builder&& other
) noexcept = default;

array_access_layout_builder::~array_access_layout_builder() = default;

array_access_layout_builder& 
array_access_layout_builder::operator=(
	array_access_layout_builder&& other
) noexcept = default;

array_access_layout_builder& array_access_layout_builder::set_extents(
	span<const std::size_t> extents
)
{
	if (m_implementation)
	{
		throw invalid_operation_error(
			"Extents can only be set once and before adding any operand"
		);
	}

	m_implementation = std::make_unique<array_access_layout_implementation>(
		array_access_layout_implementation::extent_vector_type(
			extents.begin(),
			extents.end()
		)
	);

	return *this;
}

array_access_layout_builder& array_access_layout_builder::add_operand(
	const strided_layout &layout
)
{
	array_access_layout_implementation::extent_vector_type extents;
	array_access_layout_implementation::stride_vector_type strides;
	std::ptrdiff_t offset = 0;

	const auto *layout_impl = layout.get_implementation();
	if (layout_impl)
	{
		offset = layout_impl->get_offset();
		layout_impl->get_extents(extents);
		layout_impl->get_strides(strides);
	}
	XMIPP4_ASSERT( extents.size() == strides.size() );

	if (!m_implementation)
	{
		m_implementation = 
			std::make_unique<array_access_layout_implementation>(extents);
	}

	XMIPP4_ASSERT( m_implementation );
	m_implementation->add_operand(extents, strides, offset);

	return *this;
}

const array_access_layout_implementation* 
array_access_layout_builder::get_implementation() const noexcept
{
	return m_implementation.get();
}

array_access_layout array_access_layout_builder::build(
	array_access_layout_build_flags flags
)
{
	if (m_implementation)
	{
		const auto enable_reordering = flags.contains(
			array_access_layout_build_flag_bits::enable_reordering
		);
		const auto enable_coalescing = flags.contains(
			array_access_layout_build_flag_bits::enable_coalescing
		);


		if (enable_reordering)
		{
			m_implementation->sort_axes_by_locality();
		}
		if (enable_coalescing)
		{
			m_implementation->coalesce_contiguous_axes();
		}
	}

	return array_access_layout(std::move(m_implementation));
}

} // namespace multidimensional
} // namespace xmipp4
