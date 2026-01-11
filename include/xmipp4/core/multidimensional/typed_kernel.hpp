// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "kernel.hpp"
#include "../platform/cpp_attributes.hpp"

#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Wrap a typed functor with the kernel interface.
 * 
 * @tparam Op The functor to be wrapped. Must have the a signature accepting
 * a tuple of typed operands and a pointer to a hardware queue.
 * @tparam Getter Method to extract typed handles from buffers. 
 * @tparam OutputTypeTuple List of output types.
 * @tparam InputTypeTuple Lust of input types.
 */
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

} // namespace multidimensional
} // namespace xmipp4

#include "typed_kernel.inl"
