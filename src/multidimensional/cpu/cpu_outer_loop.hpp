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
 * @brief Handles outer dimensions of multi-dimensional loops
 * 
 * @tparam Op Functor to be dispatched for 1D vectors. Must have a signature
 * accepting `(Pointers... operands, std::size_t inner_extent, 
 * InnerStrides... inner_strides)`
 * @tparam InnerStrides Variadic number of inner strides. 
 * sizeof...(InnerStrides) must equal de number of operands. 
 */
template <typename Op, typename... InnerStrides>
class cpu_outer_loop
{
public:
	using operator_type = Op;
	using inner_strides_tuple_type = std::tuple<InnerStrides...>;

	/**
	 * @brief Construct a new cpu outer loop.
	 * 
	 * @param vector_handler Function to be called for each 1D vector in the
	 * multidimensional array.
	 * @param access_layout Access layout used for iterating over the layout.
	 * @param inner_extent Number of elements in the 1D vectors.
	 * @param inner_strides Strides of the 1D vectors for each operand.
	 */
	cpu_outer_loop(
		operator_type vector_handler,
		array_access_layout access_layout,
		std::size_t inner_extent,
		const inner_strides_tuple_type &inner_strides
	);
	cpu_outer_loop(const cpu_outer_loop &other) = delete;
	cpu_outer_loop(cpu_outer_loop &&other) = default;
	~cpu_outer_loop() = default;

	cpu_outer_loop& operator=(const cpu_outer_loop &other) = delete;
	cpu_outer_loop& operator=(cpu_outer_loop &&other) = default;

	/**
	 * @brief Invoke the loop with a set of operands.
	 * 
	 * @tparam Pointers CV-qualified pointers.
	 * @param pointers The operands.
	 */
	template <typename... Pointers>
	void operator()(Pointers... pointers) const;

private:
	operator_type m_vector_handler;
	array_access_layout m_access_layout;
	std::size_t m_inner_extent;
	inner_strides_tuple_type m_inner_strides;

	template <typename... Pointers, std::size_t... Is>
	void loop_impl(
		const std::tuple<Pointers...> &pointers, 
		std::index_sequence<Is...>
	) const;

};

/**
 * @brief Construct a `cpu_outer_loop` by deducing its arguments.
 * 
 * @tparam Op Functor to be dispatched for 1D vectors. Must have a signature
 * accepting `(Pointers... operands, std::size_t inner_extent, 
 * InnerStrides... inner_strides)`
 * @tparam InnerStrides Variadic number of inner strides. 
 * sizeof...(InnerStrides) must equal de number of operands. 
 * @return cpu_outer_loop<Op, InnerStrides...> The newly created 
 * `cpu_outer_loop`
 * @param vector_handler Function to be called for each 1D vector in the
 * multidimensional array.
 * @param access_layout Access layout used for iterating over the layout.
 * @param inner_extent Number of elements in the 1D vectors.
 * @param inner_strides Strides of the 1D vectors for each operand.
 */
template <typename Op, typename... InnerStrides>
cpu_outer_loop<Op, InnerStrides...> make_cpu_outer_loop(
	Op vector_handler,
	array_access_layout access_layout,
	std::size_t inner_extent,
	const std::tuple<InnerStrides...> &inner_strides
);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_outer_loop.inl"
