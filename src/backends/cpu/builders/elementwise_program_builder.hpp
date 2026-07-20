// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/backends/cpu/program_builder.hpp>

#include <xmipp4/core/dispatch/program_builder_registry.hpp>
#include <xmipp4/core/platform/cpp_attributes.hpp>

#include <core/dispatch/core_program_builder_registry.hpp>

#include <memory>

namespace xmipp4
{
namespace cpu
{

/**
 * @brief Generic program builder for the elementwise operation family.
 *
 * Builds a CPU program that applies a per-element kernel to every element of
 * the operands, following broadcast semantics. It owns the scaffolding shared
 * by every elementwise operation: operation type check, operand-count
 * validation, joint layout construction and type dispatch.
 *
 * @tparam Op The operation type this builder targets.
 * @tparam KernelFactory Factory producing the per-element kernel. Invoked as
 * factory(operation, output_types, input_types), where operation is the
 * concrete @p Op and the type arguments are type_list-s of the resolved
 * element types. The returned kernel is invoked with one mutable pointer per
 * output operand, in signature order, followed by one const pointer per input
 * operand, in signature order. Use default_kernel_factory for stateless
 * kernels.
 * @tparam TypeDispatcher The type dispatch policy. It resolves the runtime
 * operand data types into compile-time element types and enforces the
 * operation's typing rules.
 */
template <typename Op, typename KernelFactory, typename TypeDispatcher>
class elementwise_program_builder final
	: public program_builder
{
public:
	elementwise_program_builder() noexcept = default;
	~elementwise_program_builder() override = default;

	operation_id get_operation_id() const noexcept override;

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
 * @brief Instantiate and auto-register a CPU elementwise program builder.
 */
#define XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(name, op, kernel_factory, ...) \
	static const ::xmipp4::program_builder_registration< \
		::xmipp4::cpu::elementwise_program_builder< \
			op, \
			kernel_factory, \
			__VA_ARGS__ \
		> \
	> \
	name##_program_builder_registration( \
		::xmipp4::get_core_program_builder_registry() \
	)

#include "elementwise_program_builder.inl"
