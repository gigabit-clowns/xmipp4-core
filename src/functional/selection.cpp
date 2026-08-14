// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/functional/selection.hpp>

#include <xmipp4/core/dispatch/execute.hpp>
#include <xmipp4/ops/selection/clip_operation.hpp>
#include <xmipp4/ops/selection/where_operation.hpp>

namespace xmipp4
{

array where(
	const_array_ref condition,
	const_array_ref x,
	const_array_ref y,
	const execution_context &context,
	array *out
)
{
	return execute_ternary(ops::where_operation(), condition, x, y, context, out);
}

array clip(
	const_array_ref x,
	const_array_ref lower,
	const_array_ref upper,
	const execution_context &context,
	array *out
)
{
	return execute_ternary(ops::clip_operation(), x, lower, upper, context, out);
}

} // namespace xmipp4
