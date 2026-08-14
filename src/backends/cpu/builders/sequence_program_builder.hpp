// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/backends/cpu/program_builder.hpp>

#include <xmipp4/core/dispatch/program_builder_registry.hpp>
#include <xmipp4/core/platform/cpp_attributes.hpp>

#include <core/dispatch/core_program_builder_registry.hpp>

#include <backends/cpu/builders/type_dispatchers/rule_type_dispatcher.hpp>

#include <memory>

namespace xmipp4
{
namespace cpu
{

/**
 * @brief Generic program builder for the sequence creation family.
 *
 * Builds a CPU program that writes a one dimensional operand from nothing
 * but the position of each element. It owns the scaffolding shared by every
 * such operation: operation type check, operand-count validation, traversal
 * planning and type dispatch.
 *
 * The family is single output and takes no input. Both are checked
 * statically rather than left general: a sequence is what an operation
 * writes when it has nothing to read, and an operation writing two of them
 * at once would be two operations sharing a loop, which is a thing this
 * backend has no way to describe and no operation asking for.
 *
 * @tparam Op The operation type this builder targets.
 * @tparam KernelFactory Factory producing the generator. Invoked as
 * factory(operation, output_types, input_types), where operation is the
 * concrete @p Op and the type arguments are type_list-s of the resolved
 * element types. The returned generator is invoked once per element, with a
 * mutable pointer to the element and its position in the sequence.
 * @tparam TypeDispatcher The type dispatch policy. It resolves the runtime
 * operand data types into compile-time element types. It defaults to
 * interpreting the operation's own typing rule, so a builder only names one
 * when this backend supports a narrower set of element types than the
 * operation itself allows.
 */
template <
	typename Op,
	typename KernelFactory,
	typename TypeDispatcher = rule_type_dispatcher<typename Op::type_rule>
>
class sequence_program_builder final
	: public program_builder
{
public:
	sequence_program_builder() noexcept = default;
	~sequence_program_builder() override = default;

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

private:
	using kernel_factory_type = KernelFactory;
	using type_dispatcher_type = TypeDispatcher;

	static_assert(
		Op::output_operand_count == 1,
		"A sequence operation writes exactly one operand."
	);
	static_assert(
		Op::input_operand_count == 0,
		"A sequence operation is written from its parameters alone and "
		"takes no input operand."
	);

	XMIPP4_NO_UNIQUE_ADDRESS kernel_factory_type m_kernel_factory;
	XMIPP4_NO_UNIQUE_ADDRESS type_dispatcher_type m_type_dispatcher;
};

} // namespace cpu
} // namespace xmipp4

/**
 * @brief Instantiate and auto-register a CPU sequence program builder.
 *
 * The operand element type is dispatched by interpreting the operation's own
 * typing rule, so the builder states nothing about typing.
 *
 * @param name Identifier of the registration object.
 * @param op The operation type.
 * @param kernel_factory Factory producing the generator.
 */
#define XMIPP4_REGISTER_SEQUENCE_PROGRAM_BUILDER(name, op, kernel_factory) \
	static const ::xmipp4::program_builder_registration< \
		::xmipp4::cpu::sequence_program_builder< \
			op, \
			kernel_factory \
		> \
	> \
	name##_program_builder_registration( \
		::xmipp4::get_core_program_builder_registry() \
	)

/**
 * @brief Register a CPU sequence builder with an explicit dispatcher.
 *
 * Only needed when this backend supports a narrower set of element types
 * than the operation allows, or for an operation that does not declare a
 * typing rule yet.
 *
 * @param name Identifier of the registration object.
 * @param op The operation type.
 * @param kernel_factory Factory producing the generator.
 * @param ... The type dispatcher. It comes last so that the commas in its
 * template arguments do not split the macro arguments.
 */
#define XMIPP4_REGISTER_SEQUENCE_PROGRAM_BUILDER_EX( \
	name, op, kernel_factory, ... \
) \
	static const ::xmipp4::program_builder_registration< \
		::xmipp4::cpu::sequence_program_builder< \
			op, \
			kernel_factory, \
			__VA_ARGS__ \
		> \
	> \
	name##_program_builder_registration( \
		::xmipp4::get_core_program_builder_registry() \
	)

#include "sequence_program_builder.inl"
