// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array_access_layout.hpp>
#include <xmipp4/core/multidimensional/array_iterator.hpp>
#include <xmipp4/core/platform/cpp_attributes.hpp>

#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Handles outer dimensions of multi-dimensional reduction loop.
 * 
 */
template <typename OpInit, typename OpAcc>
class cpu_reduce_outer_loop
{
public:
	using init_operator_type = OpInit;
	using accum_operator_type = OpAcc;

	/**
	 * @brief Construct a new cpu reduce_outer loop.
	 * 
	 * @param access_layout Access layout used for iterating over the layout.
	 */
	cpu_reduce_outer_loop(
		init_operator_type vector_init_handler,
		accum_operator_type vector_accum_handler,
		array_access_layout access_layout
	);
	cpu_reduce_outer_loop(const cpu_reduce_outer_loop &other) = delete;
	cpu_reduce_outer_loop(cpu_reduce_outer_loop &&other) = default;
	~cpu_reduce_outer_loop() = default;

	cpu_reduce_outer_loop& operator=(const cpu_reduce_outer_loop &other) = delete;
	cpu_reduce_outer_loop& operator=(cpu_reduce_outer_loop &&other) = default;

	/**
	 * @brief Invoke the loop with a set of operands.
	 * 
	 * @tparam Pointers CV-qualified pointers.
	 * @param pointers The operands.
	 */
	template <typename... Pointers>
	void operator()(Pointers... pointers) const;

private:
	init_operator_type m_vector_init_handler;
	accum_operator_type m_vector_accum_handler;
	array_access_layout m_access_layout;
	std::size_t m_first_reduction_axis;

	template <typename... Pointers, std::size_t... Is>
	void loop_impl(
		const std::tuple<Pointers...> &pointers, 
		std::index_sequence<Is...>
	) const;

};

/**
 * @brief Construct a `cpu_reduce_outer_loop` by deducing its arguments.
 * 
 * @return cpu_reduce_outer_loop<OpInit, OpAcc> The newly created 
 * `cpu_reduce_outer_loop`
 * @param access_layout Access layout used for iterating over the layout.
 */
template <typename OpInit, typename OpAcc>
cpu_reduce_outer_loop<OpInit, OpAcc> make_cpu_reduce_outer_loop(
	OpInit vector_init_handler,
	OpAcc vector_accum_handler,
	array_access_layout access_layout
);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_reduce_outer_loop.inl"
