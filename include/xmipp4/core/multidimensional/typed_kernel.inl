// SPDX-License-Identifier: GPL-3.0-only

#include "typed_kernel.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op, typename Getter, typename OutputPointerTuple, typename InputPointerTuple>
inline
typed_kernel<Op, Getter, OutputPointerTuple, InputPointerTuple>::typed_kernel(
	operation_type operation,
	getter_type getter
)
	: m_operation(std::move(operation))
	, m_getter(std::move(getter))
{
}

template <typename Op, typename Getter, typename OutputPointerTuple, typename InputPointerTuple>
inline
void typed_kernel<Op, Getter, OutputPointerTuple, InputPointerTuple>::execute(
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

	invoke(
		std::tuple_cat(output_pointers, input_pointers),
		std::make_integer_sequence<
			std::tuple_size<operand_pointer_tuple_type>::value
		>()
	);
}

template <typename Op, typename Getter, typename OutputPointerTuple, typename InputPointerTuple>
template<std::size_t... Is>
inline
void typed_kernel<Op, Getter, OutputPointerTuple, InputPointerTuple>::invoke(
	const operand_pointer_tuple_type& operand_pointers,
	std::index_sequence<Is...>
) const
{
	m_operation(std::get<Is>(operand_pointers)...);
}

template <typename Op, typename Getter, typename OutputPointerTuple, typename InputPointerTuple>
template<std::size_t... Is>
inline
typename
typed_kernel<Op, Getter, OutputPointerTuple, InputPointerTuple>::output_pointer_tuple_type 
typed_kernel<Op, Getter, OutputPointerTuple, InputPointerTuple>::pack_output_pointers(
	span<const std::shared_ptr<hardware::buffer>> buffers,
	std::index_sequence<Is...>
) const
{
    if (buffers.size() != sizeof...(Is))
	{
        throw std::invalid_argument(
			"typed_kernel::execute: incorrect number of output buffers"
		);
	}

    for (std::size_t i = 0; i < buffers.size(); ++i)
	{
        if (!buffers[i])
		{
            throw std::invalid_argument(
				"typed_kernel::execute: null output buffer encountered"
			);
		}
	}

    return output_pointer_tuple_type(
        m_getter<
			typename std::tuple_element<Is, output_pointer_tuple_type>::type
		>(
            *(buffers[Is])
        )...
	);
}

template <typename Op, typename Getter, typename OutputPointerTuple, typename InputPointerTuple>
template<std::size_t... Is>
inline
typename
typed_kernel<Op, Getter, OutputPointerTuple, InputPointerTuple>::input_pointer_tuple_type 
typed_kernel<Op, Getter, OutputPointerTuple, InputPointerTuple>::pack_input_pointers(
	span<const std::shared_ptr<const hardware::buffer>> buffers,
	std::index_sequence<Is...>
) const
{
    if (buffers.size() != sizeof...(Is))
	{
        throw std::invalid_argument(
			"typed_kernel: incorrect number of input buffers"
		);
	}

    for (std::size_t i = 0; i < buffers.size(); ++i)
	{
        if (!buffers[i])
		{
            throw std::invalid_argument(
				"typed_kernel: null input buffer encountered"
			);
		}
	}

    return input_pointer_tuple_type(
        m_getter<
			typename std::tuple_element<Is, input_pointer_tuple_type>::type
		>(
            *(buffers[Is])
        )...
	);
}

} // namespace multidimensional
} // namespace xmipp4
