// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_cast.hpp>

#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/array_factory.hpp>

#include <stdexcept>

namespace xmipp4 
{
namespace multidimensional
{

array cast(
	array &source,
	numerical_type target_type,
	const execution_context &context
)
{
	const auto *source_buffer = source.get_storage();
	if (!source_buffer) 
	{
		throw std::invalid_argument(
			"cast: Source array does not have storage."
		);
	}

	const auto source_type = source.get_descriptor().get_data_type();
	if (source_type == target_type)
	{
		return source.share();
	}

	return cast_copy(source, target_type, context);
}

array cast_copy(
	array_view source,
	numerical_type target_type,
	const execution_context &context,
	array *out
)
{
	return {}; // TODO
}

} // namespace multidimensional
} // namespace xmipp4
