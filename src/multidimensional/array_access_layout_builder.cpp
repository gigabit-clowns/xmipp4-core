// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "array_access_layout_implementation.hpp"

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
	std::vector<std::size_t> extents
)
{
	if (m_implementation)
	{
		throw invalid_operation_error(
			"Extents can only be set once and before adding any operand"
		);
	}

	m_implementation = std::make_unique<array_access_layout_implementation>(
		std::move(extents)
	);

	return *this;
}

array_access_layout_builder& array_access_layout_builder::add_operand(
	const strided_layout &layout,
	numerical_type data_type
)
{
	std::vector<std::size_t> extents;
	std::vector<std::ptrdiff_t> strides;
	const auto offset = layout.get_offset();
	layout.get_extents(extents);
	layout.get_strides(strides);
	XMIPP4_ASSERT( extents.size() == strides.size() );

	if (!m_implementation)
	{
		m_implementation = 
			std::make_unique<array_access_layout_implementation>(extents);
	}

	XMIPP4_ASSERT( m_implementation );
	m_implementation->add_operand(
		std::move(extents),
		std::move(strides),
		offset,
		data_type
	);

	return *this;
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
