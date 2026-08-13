// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/platform/constexpr.hpp>
#include <xmipp4/core/span.hpp>

#include <cstddef>
#include <type_traits>

namespace xmipp4
{
namespace cpu
{
namespace detail
{

/**
 * @brief Compile time operand counts of an operation.
 *
 * Reads the counts an operation declares through its typing rule, falling
 * back to the operand index enums for operations that have not been
 * migrated to a declarative rule yet. This is what lets the operations move
 * over one at a time without touching the builders.
 *
 * @tparam Op The operation type.
 *
 * @todo Remove along with the fallback once every operation declares a
 * typing rule, and read Op::output_operand_count directly.
 */
template <typename Op, typename = void>
struct operand_count_traits
{
	static XMIPP4_CONST_CONSTEXPR std::size_t output_count =
		static_cast<std::size_t>(Op::OUTPUT_OPERAND_COUNT);
	static XMIPP4_CONST_CONSTEXPR std::size_t input_count =
		static_cast<std::size_t>(Op::INPUT_OPERAND_COUNT);
};

template <typename Op>
struct operand_count_traits<
	Op,
	decltype(void(Op::output_operand_count))
>
{
	static XMIPP4_CONST_CONSTEXPR std::size_t output_count =
		Op::output_operand_count;
	static XMIPP4_CONST_CONSTEXPR std::size_t input_count =
		Op::input_operand_count;
};

/**
 * @brief Ask a type dispatcher whether it supports some operand types.
 *
 * Dispatchers that cannot answer are assumed to support everything, which
 * preserves the behaviour they had before the question could be asked: an
 * unsupported combination still throws from build().
 *
 * @tparam Dispatcher The type dispatcher.
 *
 * @todo Remove the fallback once every dispatcher can answer.
 */
template <typename Dispatcher, typename = void>
struct dispatcher_support_query
{
	static bool is_supported(
		span<const numerical_type>,
		span<const numerical_type>
	) noexcept
	{
		return true;
	}
};

template <typename Dispatcher>
struct dispatcher_support_query<
	Dispatcher,
	decltype(void(&Dispatcher::is_supported))
>
{
	static bool is_supported(
		span<const numerical_type> output_types,
		span<const numerical_type> input_types
	) noexcept
	{
		return Dispatcher::is_supported(output_types, input_types);
	}
};

} // namespace detail
} // namespace cpu
} // namespace xmipp4
