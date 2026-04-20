// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/multi_array_access_layout_builder.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "multi_array_access_layout_implementation.hpp"
#include "strided_layout_implementation.hpp"

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

void broadcast_axis_to(
	std::size_t &extent, 
	std::ptrdiff_t &stride, 
	std::size_t target_extent
)
{
	if (extent != target_extent)
	{
		if (extent == 1)
		{
			extent = target_extent;
			stride = 0;
		}
		else
		{
			std::ostringstream oss;
			oss << "Unable to broadcast extent of " << extent
				<< " into target extent of " << target_extent << ".";
			throw std::invalid_argument(oss.str());
		}
	}
}

void broadcast_to(
	multi_array_access_layout_implementation::extent_vector_type &extents,
	multi_array_access_layout_implementation::stride_vector_type &strides,
	span<const std::size_t> target_extents
)
{
	XMIPP4_ASSERT( extents.size() == strides.size() );

	const auto n = target_extents.size();
	if (extents.size() < n )
	{
		
		const auto padding = target_extents.size() - extents.size();
		extents.insert(extents.cbegin(), padding, 1UL);
		strides.insert(strides.cbegin(), padding, 0UL);
	}

	XMIPP4_ASSERT( extents.size() == n );
	XMIPP4_ASSERT( strides.size() == n );

	for (std::size_t i = 0; i < n; ++i)
	{
		broadcast_axis_to(extents[i], strides[i], target_extents[i]);
	}
}

void create_or_add_operand(
	std::unique_ptr<multi_array_access_layout_implementation> &implementation,
	multi_array_access_layout_implementation::extent_vector_type &&extents,
	multi_array_access_layout_implementation::stride_vector_type &&strides,
	std::ptrdiff_t offset
)
{
	if (implementation)
	{
		broadcast_to(
			extents,
			strides,
			implementation->get_extents()
		);
	}
	else
	{
		implementation = 
			std::make_unique<multi_array_access_layout_implementation>(
				std::move(extents)
			);

	}

	XMIPP4_ASSERT( implementation );
	implementation->add_operand(std::move(strides), offset);
}

}

multi_array_access_layout_builder::multi_array_access_layout_builder(
) noexcept = default;

multi_array_access_layout_builder::multi_array_access_layout_builder(
	multi_array_access_layout_builder&& other
) noexcept = default;

multi_array_access_layout_builder::~multi_array_access_layout_builder(
) = default;

multi_array_access_layout_builder& 
multi_array_access_layout_builder::operator=(
	multi_array_access_layout_builder&& other
) noexcept = default;

multi_array_access_layout_builder& 
multi_array_access_layout_builder::set_extents(span<const std::size_t> extents)
{
	if (m_implementation)
	{
		throw invalid_operation_error(
			"Extents can only be set once and before adding any operand"
		);
	}

	m_implementation = 
		std::make_unique<multi_array_access_layout_implementation>(
			multi_array_access_layout_implementation::extent_vector_type(
				extents.begin(),
				extents.end()
			)
		);

	return *this;
}

multi_array_access_layout_builder& 
multi_array_access_layout_builder::add_operand(
	span<const std::size_t> extents,
	span<const std::ptrdiff_t> strides,
	std::ptrdiff_t offset
)
{
	create_or_add_operand(
		m_implementation,
		multi_array_access_layout_implementation::extent_vector_type(
			extents.begin(),
			extents.end()
		),
		multi_array_access_layout_implementation::stride_vector_type(
			strides.begin(),
			strides.end()
		),
		offset
	);

	return *this;
}

multi_array_access_layout_builder& 
multi_array_access_layout_builder::add_operand(const strided_layout &layout)
{

	const auto &layout_impl = layout.get_implementation();


	multi_array_access_layout_implementation::extent_vector_type extents;
	layout_impl.get_extents(extents);

	multi_array_access_layout_implementation::stride_vector_type strides;
	layout_impl.get_strides(strides);

	const auto offset = layout_impl.get_offset();

	create_or_add_operand(
		m_implementation,
		std::move(extents),
		std::move(strides),
		offset
	);

	return *this;
}

const multi_array_access_layout_implementation* 
multi_array_access_layout_builder::get_implementation() const noexcept
{
	return m_implementation.get();
}

multi_array_access_layout multi_array_access_layout_builder::build(
	multi_array_access_layout_build_flags flags
)
{
	if (m_implementation)
	{
		const auto enable_reordering = flags.contains(
			multi_array_access_layout_build_flag_bits::enable_reordering
		);
		const auto enable_coalescing = flags.contains(
			multi_array_access_layout_build_flag_bits::enable_coalescing
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

	return multi_array_access_layout(std::move(m_implementation));
}

} // namespace multidimensional
} // namespace xmipp4
