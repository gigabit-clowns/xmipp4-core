// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "kernel.hpp"
#include "../platform/cpp_attributes.hpp"

#include <type_traits>
#include <cstddef>

namespace xmipp4 
{

template <typename... Ts>
struct type_list {};

template <typename T>
struct type_list_size;

template <typename... Ts>
struct type_list_size<type_list<Ts...>>
	: std::integral_constant<std::size_t, sizeof...(Ts)>
{
};

template<std::size_t I, typename T>
struct type_list_element;

template<std::size_t I, typename Head, typename... Tail>
struct type_list_element<I, type_list<Head, Tail...>>
    : type_list_element<I - 1, type_list<Tail...>>
{
};
 
template<class Head, typename... Tail>
struct type_list_element<0, type_list<Head, Tail...>>
{
    using type = Head;
};

namespace multidimensional
{

/**
 * @brief Wrap a typed functor with the kernel interface.
 * 
 * @tparam Op The functor to be wrapped. Must have the a signature accepting
 * a tuple of typed operands and a pointer to a hardware queue.
 * @tparam Getter Method to extract typed handles from buffers. 
 * @tparam Outputs List of output types. Must be an specialization of
 * `xmipp4::type_list`
 * @tparam Inputs Lust of input types. Must be an specialization of
 * `xmipp4::type_list`
 */
template <typename Op, typename Getter, typename Outputs, typename Inputs>
class typed_kernel final
	: public kernel
{
public:
	using operation_type = Op;
	using getter_type = Getter;
	using output_types = Outputs;
	using input_types = Inputs;

	/**
	 * @brief Construct a new typed kernel.
	 * 
	 * @param operation Operation to be wrapped.
	 * @param getter Method to obtain typed handles from the buffer.
	 */
	typed_kernel(
		operation_type operation,
		getter_type getter
	);
	~typed_kernel() override = default;

	void execute(
		span<const std::shared_ptr<hardware::buffer>> read_write_operands,
		span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
		hardware::device_queue *queue
	) const override;

private:
	XMIPP4_NO_UNIQUE_ADDRESS operation_type m_operation;
	XMIPP4_NO_UNIQUE_ADDRESS getter_type m_getter;

	template<std::size_t... OutputIs, std::size_t... InputIs>
	void execute_impl(
		span<const std::shared_ptr<hardware::buffer>> read_write_operands,
		span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
		hardware::device_queue *queue,
		std::index_sequence<OutputIs...>,
		std::index_sequence<InputIs...>
	) const;
};

/**
 * @brief Infer types and construct a `typed_kernel`
 * 
 * @tparam Op The functor to be wrapped. Must have the a signature accepting
 * a tuple of typed operands and a pointer to a hardware queue.
 * @tparam Getter Method to extract typed handles from buffers. 
 * @tparam Outputs List of output types. Must be an specialization of
 * `xmipp4::type_list`
 * @tparam Inputs Lust of input types. Must be an specialization of
 * `xmipp4::type_list`
 * @param operation Operation to be wrapped.
 * @param getter Method to obtain typed handles from the buffer.
 * @param output_types_tag Instance of `xmipp4::type_list` specialized with the
 * output types.
 * @param input_types_tag Instance of `xmipp4::type_list` specialized with the
 * input types.
 * @return typed_kernel<Op, Getter, Outputs, Inputs> The constructed kernel.
 */
template <typename Op, typename Getter, typename Outputs, typename Inputs>
typed_kernel<Op, Getter, Outputs, Inputs> make_typed_kernel(
	Op operation, 
	Getter getter,
	Outputs output_types_tag,
	Inputs input_types_tag
);

} // namespace multidimensional
} // namespace xmipp4

#include "typed_kernel.inl"
