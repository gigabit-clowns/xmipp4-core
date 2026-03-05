// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_kernel.hpp"

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>

#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op, typename Outputs, typename Inputs>
inline
cpu_kernel<Op, Outputs, Inputs>::cpu_kernel(operation_type operation)
	: m_operation(std::move(operation))
{
}

template <typename Op, typename Outputs, typename Inputs>
inline
void cpu_kernel<Op, Outputs, Inputs>::execute(
	span<const std::shared_ptr<hardware::buffer>> read_write_operands,
	span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
	hardware::device_queue *queue
) const
{
	if (queue)
	{
		queue->wait_until_completed();
	}

	execute_impl(
		read_write_operands,
		read_only_operands,
		std::make_index_sequence<type_list_size<output_types>::value>(),
		std::make_index_sequence<type_list_size<input_types>::value>()
	);
}

template <typename Op, typename Outputs, typename Inputs>
template<std::size_t... OutputIs, std::size_t... InputIs>
inline
void cpu_kernel<Op, Outputs, Inputs>::execute_impl(
	span<const std::shared_ptr<hardware::buffer>> read_write_operands,
	span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
	std::index_sequence<OutputIs...>,
	std::index_sequence<InputIs...>
) const
{
    if (read_write_operands.size() != sizeof...(OutputIs))
	{
        throw std::invalid_argument(
			"cpu_kernel::execute: incorrect number of output buffers"
		);
	}

    for (std::size_t i = 0; i < read_write_operands.size(); ++i)
	{
        if (!read_write_operands[i])
		{
            throw std::invalid_argument(
				"cpu_kernel::execute: null output buffer encountered"
			);
		}
	}

    if (read_only_operands.size() != sizeof...(InputIs))
	{
        throw std::invalid_argument(
			"cpu_kernel::execute: incorrect number of input buffers"
		);
	}

    for (std::size_t i = 0; i < read_only_operands.size(); ++i)
	{
        if (!read_only_operands[i])
		{
            throw std::invalid_argument(
				"cpu_kernel::execute: null input buffer encountered"
			);
		}
	}

	m_operation(
		std::make_tuple(
			get_pointer<typename type_list_element<OutputIs, output_types>::type>(
				*(read_write_operands[OutputIs])
			)...,
			get_pointer<typename type_list_element<InputIs, input_types>::type>(
				*(read_only_operands[InputIs])
			)...
		)
	);
}

template <typename Op, typename Outputs, typename Inputs>
template <typename T>
inline
T* cpu_kernel<Op, Outputs, Inputs>::get_pointer(hardware::buffer &buffer)
{
	auto *result = static_cast<T*>(buffer.get_host_ptr());

	if (!result)
	{
		throw std::invalid_argument(
			"cpu_kernel: Provided output buffer is not host accessible"
		);
	}

	return result;
}

template <typename Op, typename Outputs, typename Inputs>
template <typename T>
inline
const T* 
cpu_kernel<Op, Outputs, Inputs>::get_pointer(const hardware::buffer &buffer)
{
	const auto *result = static_cast<T*>(buffer.get_host_ptr());

	if (!result)
	{
		throw std::invalid_argument(
			"cpu_kernel: Provided input buffer is not host accessible"
		);
	}

	return result;
}

template <typename Op, typename Outputs, typename Inputs>
inline
cpu_kernel<Op, Outputs, Inputs> make_typed_kernel(
	Op operation, 
	Outputs,
	Inputs
)
{
	return cpu_kernel<Op, Outputs, Inputs>(std::move(operation));
}

} // namespace multidimensional
} // namespace xmipp4
