// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "array_view.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class execution_context;

XMIPP4_CORE_API
array sum(
	array_view x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

XMIPP4_CORE_API
array max(
	array_view x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

XMIPP4_CORE_API
array min(
	array_view x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

XMIPP4_CORE_API
array mean(
	array_view x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

XMIPP4_CORE_API
array variance(
	array_view x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	std::size_t ddof = 0,
	const array_view *mean = nullptr,
	const array_view *where = nullptr,
	array *out = nullptr
);

XMIPP4_CORE_API
array stddev(
	array_view x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	std::size_t ddof = 0,
	const array_view *mean = nullptr,
	const array_view *where = nullptr,
	array *out = nullptr
);

XMIPP4_CORE_API
array sum_square(
	array_view x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

XMIPP4_CORE_API
array mean_square(
	array_view x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

XMIPP4_CORE_API
array rms(
	array_view x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

XMIPP4_CORE_API
array norm(
	array_view x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

XMIPP4_CORE_API
array sumproduct(
	array_view x,
	array_view y,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
