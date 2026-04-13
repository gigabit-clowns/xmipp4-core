// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_reduction.hpp>

namespace xmipp4 
{
namespace multidimensional
{

namespace 
{

array pop_axes(array &x, span<const std::size_t> axes)
{
	const auto &descriptor = x.get_descriptor();
	return array(
		x.share_storage(),
		array_descriptor(
			descriptor.get_layout().pop_axes(axes),
			descriptor.get_data_type()
		)
	);
}

} // anonymous namespace

array sum(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim,
	const array_view *where,
	array *out
)
{
	array result;

	if (where)
	{
		//result = execute_binary(masked_sum_operation(), x, *where, context, out);
	}
	else
	{
		// result = execute_unary(sum_operation(), x, context, out);
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}
}

} // namespace multidimensional
} // namespace xmipp4
