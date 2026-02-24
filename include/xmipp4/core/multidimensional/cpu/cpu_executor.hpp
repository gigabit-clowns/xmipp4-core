// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../array_access_layout.hpp"
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

	explicit cpu_executor(
		array_access_layout layout,
		operation_type operation = {}
	);
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
	array_access_layout m_layout;
	XMIPP4_NO_UNIQUE_ADDRESS operation_type m_operation;

	template <typename... Types, std::size_t... Is>
	void execute(
		const std::tuple<Types...> &operand_pointers,
		std::index_sequence<Is...>
	) const;
};

template<typename Op>
cpu_executor<Op> make_cpu_executor(array_access_layout layout, Op operation);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_executor.inl"
