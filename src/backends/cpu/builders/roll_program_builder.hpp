// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/backends/cpu/program_builder.hpp>

#include <xmipp4/core/dispatch/program_builder_registry.hpp>

#include <core/dispatch/core_program_builder_registry.hpp>

#include <backends/cpu/builders/program_builder_registration.hpp>

#include <memory>

namespace xmipp4
{
namespace cpu
{

/**
 * @brief CPU program builder for cyclic axis shifts (fftshift/ifftshift).
 *
 * Both operations rearrange elements along a set of axes without changing
 * shape or type, differing only in how far each axis is shifted, so they
 * share this builder; @p ShiftPolicy supplies that one difference.
 *
 * @tparam Op The operation type, declared through
 * XMIPP4_DECLARE_PARAMETRIC_OPERATION with an axiswise_operation_shape_policy.
 * @tparam ShiftPolicy Provides
 * `static std::size_t shift_amount(std::size_t extent) noexcept`, the
 * distance an axis of that extent is cyclically shifted by.
 */
template <typename Op, typename ShiftPolicy>
class roll_program_builder final
	: public program_builder
{
public:
	roll_program_builder() noexcept = default;
	~roll_program_builder() override = default;

	operation_id get_operation_id() const noexcept override;

	backend_priority get_suitability(
		const operation &operation,
		span<const operand_signature> output_signatures,
		span<const operand_signature> input_signatures,
		xmipp4::command_queue &queue
	) const override;

	std::shared_ptr<xmipp4::program> build(
		const operation &operation,
		span<const operand_signature> output_signatures,
		span<const operand_signature> input_signatures,
		xmipp4::command_queue &queue,
		program_cache *cache
	) const override;
};

} // namespace cpu
} // namespace xmipp4

/**
 * @brief Instantiate and auto-register a CPU roll program builder.
 *
 * @param name Identifier of the registration object.
 * @param op The operation type.
 * @param shift_policy The shift policy type.
 */
#define XMIPP4_REGISTER_ROLL_PROGRAM_BUILDER(name, op, shift_policy) \
	XMIPP4_REGISTER_CPU_PROGRAM_BUILDER( \
		name, \
		::xmipp4::cpu::roll_program_builder<op, shift_policy> \
	)

#include "roll_program_builder.inl"
