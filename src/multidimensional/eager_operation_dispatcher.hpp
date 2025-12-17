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

/**
 * @brief De facto implementation of the dispatcher interface.
 * 
 */
class eager_operation_dispatcher
	: public operation_dispatcher
{
public:
	explicit eager_operation_dispatcher(const kernel_manager &manager) noexcept;
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
		span<const array_view> input_operands,
		const execution_context &context
	) override;

private:
	std::reference_wrapper<const kernel_manager> m_kernel_manager;

	std::shared_ptr<kernel> prepare_kernel(
		const operation &operation,
		span<array> output_operands,
		span<const array_view> input_operands,
		const execution_context &context
	) const;

	void execute_kernel(
		const kernel &kernel,
		span<array> output_operands,
		span<const array_view> input_operands,
		const execution_context &context
	) const;

};

} // namespace multidimensional
} // namespace xmipp4
