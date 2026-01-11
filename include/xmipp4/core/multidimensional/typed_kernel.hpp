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
	typename OutputTypeTuple, 
	typename InputTypeTuple
>
class typed_kernel final
	: public kernel
{
public:
	using operation_type = Op;
	using getter_type = Getter;
	using output_types = OutputTypeTuple;
	using input_types = InputTypeTuple;

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

	template<std::size_t... OutputIs, std::size_t... InputIs>
	void execute_impl(
		span<const std::shared_ptr<hardware::buffer>> read_write_operands,
		span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
		hardware::device_queue *queue,
		std::index_sequence<OutputIs...>,
		std::index_sequence<InputIs...>
	) const;

};

} // namespace multidimensional
} // namespace xmipp4

#include "typed_kernel.inl"
