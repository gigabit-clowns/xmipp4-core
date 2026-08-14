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
 * @brief Generic program builder for the reduction operation family.
 *
 * Builds a CPU program that folds the elements of its inputs along the axes
 * the operation names. It owns the scaffolding shared by every reduction:
 * operation type check, operand-count validation, the split of the iteration
 * space into the axes that survive and the axes being folded away, and type
 * dispatch.
 *
 * The axes are a parameter of the operation rather than of the call, so the
 * whole of that split is decided once here and the program that results holds
 * no decision left to make per element.
 *
 * @tparam Op The operation type this builder targets. Must carry a
 * reduction_operation_shape_policy.
 * @tparam KernelFactory Factory producing the reduction kernel. Invoked as
 * factory(operation, output_types, input_types), where operation is the
 * concrete @p Op and the type arguments are type_list-s of the resolved
 * element types. The returned kernel describes the fold through the members
 * @ref run_reduction_loop documents. Use default_kernel_factory for stateless
 * kernels, and fold_reduction_kernel for an operation that is no more than a
 * binary fold with a neutral element.
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
class reduction_program_builder final
	: public program_builder
{
public:
	reduction_program_builder() noexcept = default;
	~reduction_program_builder() override = default;

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

	XMIPP4_NO_UNIQUE_ADDRESS kernel_factory_type m_kernel_factory;
	XMIPP4_NO_UNIQUE_ADDRESS type_dispatcher_type m_type_dispatcher;
};

} // namespace cpu
} // namespace xmipp4

/**
 * @brief Instantiate and auto-register a CPU reduction program builder.
 *
 * The operand element types are dispatched by interpreting the operation's
 * own typing rule, so the builder states nothing about typing.
 *
 * @param name Identifier of the registration object.
 * @param op The operation type.
 * @param kernel_factory Factory producing the reduction kernel.
 */
#define XMIPP4_REGISTER_REDUCTION_PROGRAM_BUILDER(name, op, kernel_factory) \
	static const ::xmipp4::program_builder_registration< \
		::xmipp4::cpu::reduction_program_builder< \
			op, \
			kernel_factory \
		> \
	> \
	name##_program_builder_registration( \
		::xmipp4::get_core_program_builder_registry() \
	)

/**
 * @brief Register a CPU reduction builder with an explicit dispatcher.
 *
 * Only needed when this backend supports a narrower set of element types
 * than the operation allows, or for an operation that does not declare a
 * typing rule yet.
 *
 * @param name Identifier of the registration object.
 * @param op The operation type.
 * @param kernel_factory Factory producing the reduction kernel.
 * @param ... The type dispatcher. It comes last so that the commas in its
 * template arguments do not split the macro arguments.
 */
#define XMIPP4_REGISTER_REDUCTION_PROGRAM_BUILDER_EX( \
	name, op, kernel_factory, ... \
) \
	static const ::xmipp4::program_builder_registration< \
		::xmipp4::cpu::reduction_program_builder< \
			op, \
			kernel_factory, \
			__VA_ARGS__ \
		> \
	> \
	name##_program_builder_registration( \
		::xmipp4::get_core_program_builder_registry() \
	)

#include "reduction_program_builder.inl"
