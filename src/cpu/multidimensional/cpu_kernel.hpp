// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/platform/cpp_attributes.hpp>

#include <memory>
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
 * const-qualified pointers to operands.
 * @tparam Outputs List of output types. Must be an specialization of
 * `xmipp4::type_list`
 * @tparam Inputs Lust of input types. Must be an specialization of
 * `xmipp4::type_list`
 */
template <typename Op, typename Outputs, typename Inputs>
class cpu_kernel final
	: public kernel
{
public:
	using operation_type = Op;
	using output_types = Outputs;
	using input_types = Inputs;

	/**
	 * @brief Construct a new typed kernel.
	 * 
	 * @param operation Operation to be wrapped.
	 */
	explicit cpu_kernel(
		operation_type operation = {}
	);
	~cpu_kernel() override = default;

	void execute(
		span<const std::shared_ptr<hardware::buffer>> read_write_operands,
		span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
		hardware::device_queue *queue
	) const override;

private:
	XMIPP4_NO_UNIQUE_ADDRESS operation_type m_operation;

	template<std::size_t... OutputIs, std::size_t... InputIs>
	void execute_impl(
		span<const std::shared_ptr<hardware::buffer>> read_write_operands,
		span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
		std::index_sequence<OutputIs...>,
		std::index_sequence<InputIs...>
	) const;

	template <typename T>
	static
	T* get_pointer(hardware::buffer &buffer);

	template <typename T>
	static
	const T* get_pointer(const hardware::buffer &buffer);
};

/**
 * @brief Infer types and construct a `cpu_kernel`
 * 
 * @tparam Op The functor to be wrapped. Must have the a signature accepting
 * const-qualified pointers to operands.
 * @tparam Outputs List of output types. Must be an specialization of
 * `xmipp4::type_list`
 * @tparam Inputs Lust of input types. Must be an specialization of
 * `xmipp4::type_list`
 * @param operation Operation to be wrapped.
 * @param output_types_tag Instance of `xmipp4::type_list` specialized with the
 * output types.
 * @param input_types_tag Instance of `xmipp4::type_list` specialized with the
 * input types.
 * @return cpu_kernel<Op, Getter, Outputs, Inputs> The constructed kernel.
 */
template <typename Op, typename Outputs, typename Inputs>
cpu_kernel<Op, Outputs, Inputs> make_typed_kernel(
	Op operation, 
	Outputs output_types_tag,
	Inputs input_types_tag
);

/**
 * @brief Infer types and construct a `cpu_kernel`
 * 
 * @tparam Op The functor to be wrapped. Must have the a signature accepting
 * const-qualified pointers to operands.
 * @tparam Outputs List of output types. Must be an specialization of
 * `xmipp4::type_list`
 * @tparam Inputs Lust of input types. Must be an specialization of
 * `xmipp4::type_list`
 * @param operation Operation to be wrapped.
 * @param output_types_tag Instance of `xmipp4::type_list` specialized with the
 * output types.
 * @param input_types_tag Instance of `xmipp4::type_list` specialized with the
 * input types.
 * @return std::shared_ptr<cpu_kernel<Op, Getter, Outputs, Inputs>> 
 * The constructed kernel.
 */
template <typename Op, typename Outputs, typename Inputs>
std::shared_ptr<cpu_kernel<Op, Outputs, Inputs>> make_typed_kernel_shared(
	Op operation, 
	Outputs output_types_tag,
	Inputs input_types_tag
);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_kernel.inl"
