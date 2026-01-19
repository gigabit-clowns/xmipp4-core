// SPDX-License-Identifier: GPL-3.0-only

#include "typed_kernel.hpp"

#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op, typename Getter, typename Outputs, typename Inputs>
inline
typed_kernel<Op, Getter, Outputs, Inputs>::typed_kernel(
	operation_type operation,
	getter_type getter
)
	: m_operation(std::move(operation))
	, m_getter(std::move(getter))
{
}

template <typename Op, typename Getter, typename Outputs, typename Inputs>
inline
void typed_kernel<Op, Getter, Outputs, Inputs>::execute(
	span<const std::shared_ptr<hardware::buffer>> read_write_operands,
	span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
	hardware::device_queue *queue
) const
{
	execute_impl(
		read_write_operands,
		read_only_operands,
		queue,
		std::make_integer_sequence<type_list_size<output_types>::value>(),
		std::make_integer_sequence<type_list_size<input_types>::value>()
	);
}

template <typename Op, typename Getter, typename Outputs, typename Inputs>
template<std::size_t... OutputIs, std::size_t... InputIs>
inline
void typed_kernel<Op, Getter, Outputs, Inputs>::execute_impl(
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
		std::forward_as_tuple(
			m_getter<typename type_list_element<OutputIs, output_types>::type>(
				*(read_write_operands[OutputIs])
			)...
		),
		std::forward_as_tuple(
			m_getter<typename type_list_element<InputIs, input_types>::type>(
				*(read_only_operands[InputIs])
			)...
		),
		queue
	);
}

} // namespace multidimensional
} // namespace xmipp4
