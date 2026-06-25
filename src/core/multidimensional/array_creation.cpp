// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_creation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operations/assignment/copy_operation.hpp>
#include <xmipp4/core/multidimensional/operations/assignment/fill_operation.hpp>
#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/execution_context.hpp>

#include <core/logger.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array empty(
	array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out
)
{
	// TODO
}

array zeros(
	array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out
)
{
	return full(
		descriptor,
		affinity,
		0,
		context,
		out
	);
}

array ones(
	array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out
)
{
	return full(
		descriptor,
		affinity,
		1,
		context,
		out
	);
}

array full(
	array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const scalar_ref &fill_value,
	const execution_context &context,
	array *out
)
{
	std::array<array, 1> outputs = { 
		empty(descriptor, affinity, context, out) 
	};

	execute(
		fill_operation(fill_value),
		make_span(outputs),
		{},
		context
	);

	return std::move(outputs[0]);
}

array copy(array_view source, const execution_context &context, array *out)
{
	return execute_unary(copy_operation(), std::move(source), context, out);
}



} // namespace multidimensional
} // namespace xmipp4
