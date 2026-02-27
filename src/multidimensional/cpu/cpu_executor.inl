// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_executor.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op>
inline
cpu_executor<Op>::cpu_executor(operation_type operation)
	: m_operation(std::move(operation))
{
}

template <typename Op>
template <typename... Types>
inline
void cpu_executor<Op>::operator()(
	const std::tuple<Types*...> &operand_pointers,
	hardware::queue* queue
) const
{
	if (queue)
	{
		queue->wait_for_completion();
	}

	m_operation(operand_pointers);
}

template<typename Op>
inline
cpu_executor<Op> make_cpu_executor(Op operation)
{
	return cpu_executor<Op>(std::move(operation));
}

} // namespace multidimensional
} // namespace xmipp4
