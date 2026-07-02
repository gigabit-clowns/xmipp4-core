// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/multi_array_access_layout.hpp>
#include <xmipp4/core/multidimensional/multi_array_iterator.hpp>
#include <xmipp4/core/platform/cpp_attributes.hpp>

#include <tuple>
#include <type_traits>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Handles outer dimensions of multi-dimensional loops
 * 
 * @tparam InnerLoop Functor to be dispatched for 1D vectors. 
 * Must have a signature accepting `(Pointers... operands, std::size_t count)`
 */
template <typename InnerLoop>
class cpu_elementwise_outer_loop
{
public:
	using inner_loop_type = InnerLoop;

	/**
	 * @brief Construct a new cpu outer loop.
	 * 
	 * @param vector_handler Function to be called for each 1D vector in the
	 * multidimensional array.
	 * @param access_layout Access layout used for iterating over the layout.
	 */
	cpu_elementwise_outer_loop(
		inner_loop_type vector_handler,
		multi_array_access_layout access_layout
	);
	cpu_elementwise_outer_loop(const cpu_elementwise_outer_loop &other) = delete;
	cpu_elementwise_outer_loop(cpu_elementwise_outer_loop &&other) = default;
	~cpu_elementwise_outer_loop() = default;

	cpu_elementwise_outer_loop& 
	operator=(const cpu_elementwise_outer_loop &other) = delete;
	cpu_elementwise_outer_loop& 
	operator=(cpu_elementwise_outer_loop &&other) = default;

	/**
	 * @brief Invoke the loop with a set of operands.
	 * 
	 * @tparam Pointers CV-qualified pointers.
	 * @param pointers The operands.
	 */
	template <typename... Pointers>
	void operator()(Pointers... pointers) const;

private:
	inner_loop_type m_vector_handler;
	multi_array_access_layout m_access_layout;

	template <typename... Pointers, std::size_t... Is>
	void loop_impl(
		const std::tuple<Pointers...> &pointers, 
		std::index_sequence<Is...>
	) const;
};

/**
 * @brief Construct a `cpu_elementwise_outer_loop` by deducing its arguments.
 * 
 * @tparam InnerLoop Functor to be dispatched for 1D vectors. Must have a 
 * signature accepting `(Pointers... operands, std::size_t count)`
 * @return cpu_elementwise_outer_loop<InnerLoop> The newly created 
 * `cpu_elementwise_outer_loop`
 * @param vector_handler Function to be called for each 1D vector in the
 * multidimensional array.
 * @param access_layout Access layout used for iterating over the layout.
 */
template <typename InnerLoop>
cpu_elementwise_outer_loop<typename std::decay<InnerLoop>::type>
make_cpu_outer_loop(
	InnerLoop&& vector_handler,
	multi_array_access_layout access_layout
);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_elementwise_outer_loop.inl"
