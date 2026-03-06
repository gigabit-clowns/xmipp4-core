// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array_access_layout.hpp>

#include <type_traits>
#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

template <typename F, std::size_t N>
auto dispatch_inner_loop(
	F &&callable, 
	const array_access_layout &layout, 
	std::integral_constant<std::size_t, N> n_operands
);

template <typename F, std::size_t... Is>
auto dispatch_inner_loop(
	F &&callable, 
	const array_access_layout &layout, 
	std::index_sequence<Is...> operand_indices
);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_inner_loop_dispatch.inl"
