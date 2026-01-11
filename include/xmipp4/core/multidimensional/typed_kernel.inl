// SPDX-License-Identifier: GPL-3.0-only

#include "typed_kernel.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op, typename Getter, typename OutputTypeTuple, typename InputTypeTuple>
inline
typed_kernel<Op, Getter, OutputTypeTuple, InputTypeTuple>::typed_kernel(
	operation_type operation,
	getter_type getter
)
	: m_operation(std::move(operation))
	, m_getter(std::move(getter))
{
}

template <typename Op, typename Getter, typename OutputTypeTuple, typename InputTypeTuple>
inline
void typed_kernel<Op, Getter, OutputTypeTuple, InputTypeTuple>::execute(
	span<const std::shared_ptr<hardware::buffer>> read_write_operands,
	span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
	hardware::device_queue *queue
) const
{
	execute_impl(
		read_write_operands,
		read_only_operands,
		queue,
		std::make_integer_sequence<std::tuple_size<output_types>::value>(),
		std::make_integer_sequence<std::tuple_size<input_types>::value>()
	);
}

template <typename Op, typename Getter, typename OutputTypeTuple, typename InputTypeTuple>
template<std::size_t... OutputIs, std::size_t... InputIs>
inline
void typed_kernel<Op, Getter, OutputTypeTuple, InputTypeTuple>::execute_impl(
	span<const std::shared_ptr<hardware::buffer>> read_write_operands,
	span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
	hardware::device_queue *queue,
	std::index_sequence<OutputIs...>,
	std::index_sequence<InputIs...>
) const
{
    if (read_write_operands.size() != sizeof...(OutputIs))
	{
        throw std::invalid_argument(
			"typed_kernel::execute: incorrect number of output buffers"
		);
	}

    for (std::size_t i = 0; i < read_write_operands.size(); ++i)
	{
        if (!read_write_operands[i])
		{
            throw std::invalid_argument(
				"typed_kernel::execute: null output buffer encountered"
			);
		}
	}

    if (read_only_operands.size() != sizeof...(InputIs))
	{
        throw std::invalid_argument(
			"typed_kernel::execute: incorrect number of input buffers"
		);
	}

    for (std::size_t i = 0; i < read_only_operands.size(); ++i)
	{
        if (!read_only_operands[i])
		{
            throw std::invalid_argument(
				"typed_kernel::execute: null input buffer encountered"
			);
		}
	}

	m_operation(
		queue, // TODO decide where to pass
        m_getter<typename std::tuple_element<OutputIs, output_types>::type>(
            *(read_write_operands[OutputIs])
        )...,
        m_getter<typename std::tuple_element<InputIs, input_types>::type>(
            *(read_only_operands[InputIs])
        )...
	);
}

} // namespace multidimensional
} // namespace xmipp4
