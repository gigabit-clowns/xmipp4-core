// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_kernel.hpp"

#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op, typename OutputPointerTuple, typename InputPointerTuple>
inline
cpu_kernel<Op, OutputPointerTuple, InputPointerTuple>::cpu_kernel(
	operation_type operation,
	array_access_layout access_layout
)
	: m_operation(std::move(operation))
	, m_access_layout(std::move(access_layout))
{
}

template <typename Op, typename OutputPointerTuple, typename InputPointerTuple>
inline
void cpu_kernel<Op, OutputPointerTuple, InputPointerTuple>::execute(
	span<const std::shared_ptr<hardware::buffer>> read_write_operands,
	span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
	hardware::device_queue *queue
) const
{
	const auto output_pointers = pack_output_pointers(
		read_write_operands,
		std::make_integer_sequence<
			std::tuple_size<output_pointer_tuple_type>::value
		>()
	);
	const auto input_pointers = pack_input_pointers(
		read_only_operands,
		std::make_integer_sequence<
			std::tuple_size<input_pointer_tuple_type>::value
		>()
	);

	if (queue)
	{
		queue->wait_until_completed();
	}

	loop(
		std::tuple_cat(output_pointers, input_pointers),
		std::make_integer_sequence<
			std::tuple_size<operand_pointer_tuple_type>::value
		>()
	);
}

template <typename Op, typename OutputPointerTuple, typename InputPointerTuple>
template<std::size_t... Is>
inline
void cpu_kernel<Op, OutputPointerTuple, InputPointerTuple>::loop(
	const operand_pointer_tuple_type& operand_pointers,
	std::index_sequence<Is...>
) const
{
	array_iterator ite;
	std::size_t count;
	if (!(count = m_access_layout.iter(ite)))
	{
		return;
	}

	const auto offsets = ite.get_offsets(); // Stable address
	do
	{
		m_operation(
			count,
			std::get<Is>(operand_pointers) + offsets[Is]...
		);
	}
	while((count = m_access_layout.next(ite, count)));
}

template <typename Op, typename OutputPointerTuple, typename InputPointerTuple>
template<std::size_t... Is>
inline
typename
cpu_kernel<Op, OutputPointerTuple, InputPointerTuple>::output_pointer_tuple_type 
cpu_kernel<Op, OutputPointerTuple, InputPointerTuple>::pack_output_pointers(
	span<const std::shared_ptr<hardware::buffer>> buffers,
	std::index_sequence<Is...>
)
{
    if (buffers.size() != sizeof...(Is))
	{
        throw std::invalid_argument(
			"cpu_kernel: incorrect number of output buffers"
		);
	}

    for (std::size_t i = 0; i < buffers.size(); ++i)
	{
        if (!buffers[i])
		{
            throw std::invalid_argument(
				"cpu_kernel: null output buffer encountered"
			);
		}
	}

    return output_pointer_tuple_type(
        static_cast<
			typename std::tuple_element<Is, output_pointer_tuple_type>::type
		>(
            buffers[Is]->get_host_ptr()
        )...
	);
}

template <typename Op, typename OutputPointerTuple, typename InputPointerTuple>
template<std::size_t... Is>
inline
typename
cpu_kernel<Op, OutputPointerTuple, InputPointerTuple>::input_pointer_tuple_type 
cpu_kernel<Op, OutputPointerTuple, InputPointerTuple>::pack_input_pointers(
	span<const std::shared_ptr<const hardware::buffer>> buffers,
	std::index_sequence<Is...>
)
{
    if (buffers.size() != sizeof...(Is))
	{
        throw std::invalid_argument(
			"cpu_kernel: incorrect number of input buffers"
		);
	}

    for (std::size_t i = 0; i < buffers.size(); ++i)
	{
        if (!buffers[i])
		{
            throw std::invalid_argument(
				"cpu_kernel: null input buffer encountered"
			);
		}
	}

    return input_pointer_tuple_type(
        static_cast<
			typename std::tuple_element<Is, input_pointer_tuple_type>::type
		>(
            buffers[Is]->get_host_ptr()
        )...
	);
}

} // namespace multidimensional
} // namespace xmipp4
