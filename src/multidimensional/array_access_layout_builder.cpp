// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include "array_access_layout_implementation.hpp"

namespace xmipp4 
{
namespace multidimensional
{

const array_access_layout_build_flags 
array_access_layout_builder::default_flags = {
	array_access_layout_build_flag_bits::reorder_batches,
	array_access_layout_build_flag_bits::coalesce_batches
};

array_access_layout_builder::array_access_layout_builder() noexcept = default;

array_access_layout_builder::array_access_layout_builder(
	std::vector<std::size_t> batch_extents
)
	: m_implementation(
		std::make_unique<array_access_layout_implementation>(
			std::move(batch_extents)
		)
	)
{
}

array_access_layout_builder::array_access_layout_builder(
	array_access_layout_builder&& other
) noexcept = default;

array_access_layout_builder::~array_access_layout_builder() = default;

array_access_layout_builder& 
array_access_layout_builder::operator=(
	array_access_layout_builder&& other
) noexcept = default;

array_access_layout_builder& array_access_layout_builder::add_operand(
	const strided_layout &layout,
	numerical_type data_type,
	std::size_t core_dimensions 
)
{
	if (core_dimensions > layout.get_rank())
	{
		throw std::invalid_argument(
			"core_dimensions is out of bounds"
		);
	}

	std::vector<std::size_t> extents;
	std::vector<std::ptrdiff_t> strides;
	const auto offset = layout.get_offset();
	layout.get_extents(extents);
	layout.get_strides(strides);
	XMIPP4_ASSERT( extents.size() == strides.size() );

	if (!m_implementation)
	{
		std::vector<std::size_t> batch_extents(
			extents.cbegin(), 
			std::prev(extents.cend(), core_dimensions)
		);

		m_implementation = std::make_unique<array_access_layout_implementation>(
			std::move(batch_extents)
		);
	}

	XMIPP4_ASSERT( m_implementation );
	m_implementation->add_operand(
		std::move(extents),
		std::move(strides),
		offset,
		data_type,
		core_dimensions
	);

	return *this;
}

array_access_layout array_access_layout_builder::build(
	array_access_layout_build_flags flags
)
{
	if (m_implementation)
	{
		if (flags.contains(array_access_layout_build_flag_bits::reorder_batches))
		{
			m_implementation->sort_batch_axes_by_locality();
		}
		
		if (flags.contains(array_access_layout_build_flag_bits::coalesce_batches))
		{
			m_implementation->coalesce_contiguous_batch_axes();
		}
	}

	return array_access_layout(std::move(m_implementation));
}

} // namespace multidimensional
} // namespace xmipp4
