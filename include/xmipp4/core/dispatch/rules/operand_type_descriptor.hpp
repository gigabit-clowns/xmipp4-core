// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operand_type_pivot.hpp"
#include "operand_type_slot.hpp"

#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/numerical/numerical_type_domain.hpp>
#include <xmipp4/core/span.hpp>

#include <array>
#include <cstddef>

namespace xmipp4
{

/**
 * @brief Type erased form of an operand_type_pivot.
 *
 * A typing rule is a compile time declaration, but interpreting it at
 * runtime does not need any of its types. Lowering it to a table of plain
 * descriptors means the interpreter is one ordinary function rather than
 * one instantiation per rule, which matters when the catalog holds
 * hundreds of operations.
 *
 * @see operand_type_pivot
 */
struct pivot_descriptor
{
	/// Where the pivot value comes from.
	pivot_source source;

	/// Index of the operand, or of the operation supplied value.
	std::size_t index;

	/// Pivot whose value is adopted by default, or -1 for none.
	std::ptrdiff_t default_pivot;

	/// Set of types the pivot may take. Never null.
	const numerical_type_domain *domain;
};

/**
 * @brief Type erased form of an operand_type_slot.
 *
 * @see operand_type_slot
 */
struct slot_descriptor
{
	/// Index of the pivot the operand derives from.
	std::size_t pivot_index;

	/// Value level half of the transform. Never null.
	numerical_type (*apply)(numerical_type);

	/// Set of pivot types the transform is defined on. Never null.
	const numerical_type_domain *transform_domain;

	/// Fragment naming the mapping, for diagnostics. Never null.
	const char *relation;
};

/**
 * @brief Lower a rule's pivot list into a table of descriptors.
 *
 * @tparam PivotList type_list of operand_type_pivot declarations.
 */
template <typename PivotList>
struct pivot_descriptor_table;

template <typename... Pivots>
struct pivot_descriptor_table<type_list<Pivots...>>
{
	static span<const pivot_descriptor> get() noexcept
	{
		static const std::array<pivot_descriptor, sizeof...(Pivots)> table =
		{{
			pivot_descriptor
			{
				Pivots::source,
				Pivots::index,
				Pivots::default_pivot,
				&Pivots::domain_type::get()
			}...
		}};

		return make_span(table.data(), table.size());
	}
};

/**
 * @brief Lower a rule's slot list into a table of descriptors.
 *
 * @tparam SlotList type_list of operand_type_slot declarations.
 */
template <typename SlotList>
struct slot_descriptor_table;

template <>
struct slot_descriptor_table<type_list<>>
{
	static span<const slot_descriptor> get() noexcept
	{
		return span<const slot_descriptor>();
	}
};

template <typename... Slots>
struct slot_descriptor_table<type_list<Slots...>>
{
	static span<const slot_descriptor> get() noexcept
	{
		static const std::array<slot_descriptor, sizeof...(Slots)> table =
		{{
			slot_descriptor
			{
				Slots::pivot_index,
				&Slots::transform_type::apply,
				&Slots::transform_type::domain_type::get(),
				Slots::transform_type::get_relation()
			}...
		}};

		return make_span(table.data(), table.size());
	}
};

} // namespace xmipp4
