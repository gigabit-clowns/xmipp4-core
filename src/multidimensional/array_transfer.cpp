// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_transfer.hpp>

#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/array_factory.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/memory_resource.hpp>

#include <stdexcept>

namespace xmipp4 
{
namespace multidimensional
{

array transfer(
	array &source,
	hardware::memory_resource_affinity affinity,
	const execution_context &context
)
{
	const auto *source_buffer = source.get_storage();
	if (!source_buffer) 
	{
		throw std::invalid_argument(
			"transfer: Source array does not have storage."
		);
	}
	const auto &source_resource = source_buffer->get_memory_resource();

	const auto &destination_allocator = context.get_memory_allocator(
		affinity
	);
	const auto &destination_resource = 
		destination_allocator.get_memory_resource();

	if (&source_resource == &destination_resource)
	{
		return source.share();
	}

	return transfer_copy(source, affinity, context);
}

array transfer_copy(
	array_view source,
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out
)
{
	auto destination = empty(source.get_descriptor(), affinity, context, out);

	// TODO dispatch a transfer operation

	return destination;
}

} // namespace multidimensional
} // namespace xmipp4
