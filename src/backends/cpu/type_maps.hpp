// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/numerical/numerical_type_dispatch.hpp>

#include <half.hpp>

#include <complex>

namespace xmipp4
{
namespace cpu
{

/**
 * Element types used by CPU programs when computing each numerical_type.
 *
 * The public float16_t is a storage-only placeholder without arithmetic
 * support, so the CPU backend computes half precision with
 * half_float::half. The remaining numerical types are computed with
 * their native representations.
 *
 * SIMD fast paths (e.g. Google Highway kernels on contiguous operands)
 * do not dispatch through these maps: they adapt bit-compatible element
 * types (such as hwy::float16_t) at their call boundary instead.
 */

using half_float16_entry =
	type_map_entry<numerical_type::float16, half_float::half>;

using half_complex_float16_entry = type_map_entry<
	numerical_type::complex_float16,
	std::complex<half_float::half>
>;

using compute_floating_type_map = type_map<
	half_float16_entry,
	native_float32_entry,
	native_float64_entry
>;

using compute_complex_type_map = type_map<
	half_complex_float16_entry,
	native_complex_float32_entry,
	native_complex_float64_entry
>;

using compute_arithmetic_type_map = type_map_cat<
	native_integer_type_map,
	compute_floating_type_map,
	compute_complex_type_map
>::type;

using compute_type_map = type_map_cat<
	compute_arithmetic_type_map,
	type_map<native_boolean_entry, native_char8_entry>
>::type;

} // namespace cpu
} // namespace xmipp4
