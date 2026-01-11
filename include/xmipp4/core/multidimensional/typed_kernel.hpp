// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "kernel.hpp"

#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

template <
	typename Op, 
	typename Getter,
	typename OutputPointerTuple, 
	typename InputPointerTuple
>
class typed_kernel final
	: public kernel
{
public:
	using operation_type = Op;
	using getter_type = Getter;
	using output_pointer_tuple_type = OutputPointerTuple;
	using input_pointer_tuple_type = InputPointerTuple;
	using operand_pointer_tuple_type = decltype(std::tuple_cat(
		std::declval<output_pointer_tuple_type>(),
		std::declval<input_pointer_tuple_type>()
	));

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
	operation_type m_operation;
	getter_type m_getter;

	template<std::size_t... Is>
	void invoke(
		const operand_pointer_tuple_type &operand_pointers,
		std::index_sequence<Is...>
	) const;

	template<std::size_t... Is>
	output_pointer_tuple_type pack_output_pointers(
		span<const std::shared_ptr<hardware::buffer>> buffers,
		std::index_sequence<Is...>
	) const;

	template<std::size_t... Is>
	input_pointer_tuple_type pack_input_pointers(
		span<const std::shared_ptr<const hardware::buffer>> buffers,
		std::index_sequence<Is...>
	) const;
};

} // namespace multidimensional
} // namespace xmipp4

#include "typed_kernel.inl"
