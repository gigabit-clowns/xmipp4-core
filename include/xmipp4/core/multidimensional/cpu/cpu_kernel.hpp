// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../kernel.hpp"
#include "../array_access_layout.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <
	typename Op, 
	typename OutputPointerTuple, 
	typename InputPointerTuple
>
class cpu_kernel final
	: public kernel
{
public:
	using operation_type = Op;
	using output_pointer_tuple_type = OutputPointerTuple;
	using input_pointer_tuple_type = InputPointerTuple;

	cpu_kernel(
		operation_type operation,
		array_access_layout access_layout
	);
	~cpu_kernel() override = default;

	void execute(
		span<const std::shared_ptr<hardware::buffer>> read_write_operands,
		span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
		hardware::device_queue *queue
	) const override;

private:
	operation_type m_operation;
	array_access_layout m_access_layout;

	void loop(
		const output_pointer_tuple_type &output_pointers,
		const input_pointer_tuple_type &input_pointers
	);
};

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_kernel.inl"
