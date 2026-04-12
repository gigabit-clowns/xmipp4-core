// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/multi_array_access_layout.hpp>
#include <xmipp4/core/multidimensional/multi_array_iterator.hpp>
#include <xmipp4/core/platform/cpp_attributes.hpp>

#include <tuple>
#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Handles outer dimensions of multi-dimensional reduction loop.
 * 
 * @tparam OpInit Functor to be dispatched for initializing 1D vectors. 
 * Must have a signature accepting `(Pointers... operands, std::size_t count)`
 * @tparam OpAccum Functor to be dispatched for accumulation 1D vectors. 
 * Must have a signature accepting `(Pointers... operands, std::size_t count)`
 */
template <typename OpInit, typename OpAccum>
class cpu_reduce_outer_loop
{
public:
	using init_functor_type = OpInit;
	using accum_functor_type = OpAccum;

	/**
	 * @brief Construct a new cpu reduce_outer loop.
	 * 
	 * @param vector_init_handler Functor to be called for the first iteration
	 * on the reduction loop. It should not read from the output values and
	 * instead only write the result of the first iteration given the first 
	 * input vector.
	 * @param vector_accum_handler Functor to be called in subsequent reduction
	 * iterations. It should apply to the output the result of the current
	 * iteration.
	 * @param access_layout Access layout used for iterating over the layout.
	 */
	cpu_reduce_outer_loop(
		init_functor_type vector_init_handler,
		accum_functor_type vector_accum_handler,
		multi_array_access_layout access_layout
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
	init_functor_type m_vector_init_handler;
	accum_functor_type m_vector_accum_handler;
	multi_array_access_layout m_access_layout;
	std::vector<std::size_t> m_reduce_axes;

	template <typename... Pointers, std::size_t... Is>
	void loop_impl(
		const std::tuple<Pointers...> &pointers, 
		std::index_sequence<Is...>
	) const;

	bool is_first_iter(span<const std::size_t> indices) const noexcept;

};

/**
 * @brief Construct a `cpu_reduce_outer_loop` by deducing its arguments.
 * 
 * @tparam OpInit Functor to be dispatched for initializing 1D vectors. 
 * Must have a signature accepting `(Pointers... operands, std::size_t count)`
 * @tparam OpAccum Functor to be dispatched for accumulation 1D vectors. 
 * Must have a signature accepting `(Pointers... operands, std::size_t count)`
 * 
 * @param vector_init_handler Functor to be called for the first iteration
 * on the reduction loop. It should not read from the output values and
 * instead only write the result of the first iteration given the first 
 * input vector.
 * @param vector_accum_handler Functor to be called in subsequent reduction
 * iterations. It should apply to the output the result of the current
 * iteration.
 * @param access_layout Access layout used for iterating over the layout.
 * @return cpu_reduce_outer_loop<Op> The newly created `cpu_reduce_outer_loop`
 */
template <typename OpInit, typename OpAccum>
cpu_reduce_outer_loop<OpInit, OpAccum> make_cpu_reduce_outer_loop(
	OpInit vector_init_handler,
	OpAccum vector_accum_handler,
	multi_array_access_layout access_layout
);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_reduce_outer_loop.inl"
