// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_reduction.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array sum(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim,
	const array_view *where,
	array *out
)
{

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

}

} // namespace multidimensional
} // namespace xmipp4
