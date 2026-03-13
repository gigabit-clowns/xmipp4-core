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
 * accepting `(Pointers... operands)`
 */
template <typename Op>
class cpu_outer_loop
{
public:
	using operator_type = Op;

	/**
	 * @brief Construct a new cpu outer loop.
	 * 
	 * @param vector_handler Function to be called for each 1D vector in the
	 * multidimensional array.
	 * @param access_layout Access layout used for iterating over the layout.
	 */
	cpu_outer_loop(
		operator_type vector_handler,
		array_access_layout access_layout
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
 * accepting `(Pointers... operands)`
 * @return cpu_outer_loop<Op> The newly created 
 * `cpu_outer_loop`
 * @param vector_handler Function to be called for each 1D vector in the
 * multidimensional array.
 * @param access_layout Access layout used for iterating over the layout.
 */
template <typename Op>
cpu_outer_loop<Op> make_cpu_outer_loop(
	Op vector_handler,
	array_access_layout access_layout
);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_outer_loop.inl"
