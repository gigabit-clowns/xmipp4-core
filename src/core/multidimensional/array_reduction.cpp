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

/*
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
		//result = execute_binary(masked_sum_operation(), x, *where, context, out); // TODO
	}
	else
	{
		// result = execute_unary(sum_operation(), x, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array max(
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
		//result = execute_binary(masked_max_operation(), x, *where, context, out);
	}
	else
	{
		// result = execute_unary(max_operation(), x, context, out);
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array min(
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
		//result = execute_binary(masked_min_operation(), x, *where, context, out); // TODO
	}
	else
	{
		// result = execute_unary(min_operation(), x, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array mean(
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
		//result = execute_binary(masked_mean_operation(), x, *where, context, out); // TODO
	}
	else
	{
		// result = execute_unary(mean_operation(), x, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}


array variance(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim,
	const array_view *mean,
	const array_view *where,
	array *out
)
{
	if (!mean)
	{
		const auto m = array_view(
			multidimensional::mean(
				x,
				axes,
				context,
				true,
				where
			)
		);
		return variance(x, axes, context, keep_dim, &m, where, out);
	}

	array result;

	if (where)
	{
		//result = execute_ternary(masked_variance_operation(), x, *mean, *where, context, out); // TODO
	}
	else
	{
		// result = execute_binary(variance_operation(), x, *mean, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array stddev(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim,
	const array_view *mean,
	const array_view *where,
	array *out
)
{
	if (!mean)
	{
		const auto m = array_view(
			multidimensional::mean(
				x,
				axes,
				context,
				true,
				where
			)
		);
		return variance(x, axes, context, keep_dim, &m, where, out);
	}

	array result;

	if (where)
	{
		//result = execute_ternary(masked_variance_operation(), x, *mean, *where, context, out); // TODO
	}
	else
	{
		// result = execute_binary(variance_operation(), x, *mean, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array energy(
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
		//result = execute_binary(masked_energy_operation(), x, *where, context, out); // TODO
	}
	else
	{
		// result = execute_unary(energy_operation(), x, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array power(
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
		//result = execute_binary(masked_power_operation(), x, *where, context, out); // TODO
	}
	else
	{
		// result = execute_unary(power_operation(), x, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array rms(
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
		//result = execute_binary(masked_rms_operation(), x, *where, context, out); // TODO
	}
	else
	{
		// result = execute_unary(rms_operation(), x, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array norm(
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
		//result = execute_binary(masked_norm_operation(), x, *where, context, out); // TODO
	}
	else
	{
		// result = execute_unary(norm_operation(), x, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array sumproduct(
	const array_view &x,
	const array_view &y,
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
		//result = execute_ternary(masked_sumproduct_operation(), x, y, *where, context, out); // TODO
	}
	else
	{
		// result = execute_binary(sumproduct_operation(), x, y, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array euclidean_distance(
	const array_view &x,
	const array_view &y,
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
		//result = execute_ternary(masked_euclidean_distance_operation(), x, y, *where, context, out); // TODO
	}
	else
	{
		// result = execute_binary(euclidean_distance_operation(), x, y, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}

array euclidean_distance2(
	const array_view &x,
	const array_view &y,
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
		//result = execute_ternary(masked_euclidean_distance2_operation(), x, y, *where, context, out); // TODO
	}
	else
	{
		// result = execute_binary(euclidean_distance2_operation(), x, y, context, out); // TODO
	}

	if (!keep_dim)
	{
		result = pop_axes(result, axes);
	}

	return result;
}
*/
} // namespace multidimensional
} // namespace xmipp4
