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
class cpu_loop
{
public:
	using operation_type = Op;

	explicit cpu_loop(
		array_access_layout layout,
		operation_type operation = {}
	);
	cpu_loop(const cpu_loop &other) = default;
	cpu_loop(cpu_loop &&other) = default;
	~cpu_loop() = default;

	cpu_loop& operator=(const cpu_loop &other) = default;
	cpu_loop& operator=(cpu_loop &&other) = default;

	template <typename... OutputTypes, typename... InputTypes>
	void operator()(
		const std::tuple<OutputTypes*...> &output_pointers,
		const std::tuple<const InputTypes*...> &input_pointers,
		hardware::queue *queue
	) const;

private:
	array_access_layout m_layout;
	XMIPP4_NO_UNIQUE_ADDRESS operation_type m_operation;

	template <typename... Pointers, std::size_t... Is>
	void loop(
		const std::tuple<Pointers...> &pointers,
		std::index_sequence<Is...>
	) const;
};

template<typename Op>
cpu_loop<Op> make_cpu_loop(array_access_layout layout, Op operation = {});

} // namespace multidimensional
} // namespace xmipp4
