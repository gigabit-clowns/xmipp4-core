// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../../platform/attributes.hpp"

#include <tuple>
#include <type_traits>
#include <cstddef>

namespace xmipp4 
{
namespace hardware
{

class queue;

} // namespace hardware
namespace multidimensional
{

template <typename Op>
class cpu_executor
{
public:
	using operation_type = Op;

	explicit cpu_executor(operation_type operation = {});
	cpu_executor(const cpu_executor &other) = default;
	cpu_executor(cpu_executor &&other) = default;
	~cpu_executor() = default;

	cpu_executor& operator=(const cpu_executor &other) = default;
	cpu_executor& operator=(cpu_executor &&other) = default;

	template <typename... Types>
	void operator()(
		const std::tuple<Types*...> &operand_pointers,
		hardware::queue *queue
	) const;

private:
	XMIPP4_NO_UNIQUE_ADDRESS operation_type m_operation;
};

template<typename Op>
cpu_executor<Op> make_cpu_executor(Op operation);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_executor.inl"
