// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class operation;
class array;
class kernel;
class kernel_manager;

class eager_operation_dispatcher
	: operation_dispatcher
{
public:
	eager_operation_dispatcher(kernel_manager &manager) noexcept;
	eager_operation_dispatcher(const eager_operation_dispatcher &other) = delete;
	eager_operation_dispatcher(eager_operation_dispatcher &&other) = delete;
	~eager_operation_dispatcher() override;

	eager_operation_dispatcher& 
	operator=(const eager_operation_dispatcher &other) = delete;
	eager_operation_dispatcher& 
	operator=(eager_operation_dispatcher &&other) = delete;

	void dispatch(
		const operation &operation,
		span<array> output_operands,
		span<const array> input_operands,
		const execution_context &context
	) override;

private:
	std::reference_wrapper<kernel_manager> m_kernel_manager;

	std::shared_ptr<kernel> prepare_kernel(
		const operation &operation,
		span<array> output_operands,
		span<const array> input_operands,
		const execution_context &context
	);

	void execute_kernel(
		kernel &kernel,
		span<array> output_operands,
		span<const array> input_operands,
		const execution_context &context
	);

};

} // namespace multidimensional
} // namespace xmipp4
