// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../array_access_layout.hpp"
#include "../../platform/attributes.hpp"

#include <tuple>
#include <type_traits>
#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op>
class cpu_outer_loop
{
public:
	using operation_type = Op;

	explicit cpu_outer_loop(
		array_access_layout layout,
		operation_type operation = {}
	);
	cpu_outer_loop(const cpu_outer_loop &other) = default;
	cpu_outer_loop(cpu_outer_loop &&other) = default;
	~cpu_outer_loop() = default;

	cpu_outer_loop& operator=(const cpu_outer_loop &other) = default;
	cpu_outer_loop& operator=(cpu_outer_loop &&other) = default;

	template <typename... Types>
	void operator()(const std::tuple<Types*...> &operand_pointers) const;

private:
	array_access_layout m_layout;
	XMIPP4_NO_UNIQUE_ADDRESS operation_type m_operation;

	template <typename... Types, std::size_t... Is>
	void loop(
		const std::tuple<Types...> &operand_pointers,
		std::index_sequence<Is...>
	) const;
};

template<typename Op>
cpu_outer_loop<Op> make_cpu_outer_loop(array_access_layout layout, Op operation);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_outer_loop.inl"
