// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/kernel_access_layout_builder.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include "kernel_access_layout_implementation.hpp"

namespace xmipp4 
{
namespace multidimensional
{

kernel_access_layout_builder::kernel_access_layout_builder() noexcept = default;

kernel_access_layout_builder::kernel_access_layout_builder(
	std::vector<std::size_t> batch_extents
)
	: m_implementation(
		std::make_unique<kernel_access_layout_implementation>(
			std::move(batch_extents)
		)
	)
{
}

kernel_access_layout_builder::kernel_access_layout_builder(
	kernel_access_layout_builder&& other
) noexcept = default;

kernel_access_layout_builder::~kernel_access_layout_builder() = default;

kernel_access_layout_builder& 
kernel_access_layout_builder::operator=(
	kernel_access_layout_builder&& other
) noexcept = default;

kernel_access_layout_builder& kernel_access_layout_builder::add_operand(
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

		m_implementation = std::make_unique<kernel_access_layout_implementation>(
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

kernel_access_layout kernel_access_layout_builder::build()
{
	if (m_implementation)
	{
		m_implementation->sort_batch_axes_by_locality();
		m_implementation->coalesce_contiguous_batch_axes();
	}

	return kernel_access_layout(std::move(m_implementation));
}

} // namespace multidimensional
} // namespace xmipp4
