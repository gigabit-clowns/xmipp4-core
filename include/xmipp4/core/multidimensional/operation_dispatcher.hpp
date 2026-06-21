// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"

#include <memory>

namespace xmipp4
{

namespace hardware
{

class device_context;

} // namespace hardware

namespace multidimensional
{

class operation;
class array;
class array_view;
class operation_program_manager;

/**
 * @brief Abstract interface that executes operations on a set of operands.
 *
 * A dispatcher takes the immutable description of an operation together with
 * its input and output operands and turns it into work submitted to a device.
 * It owns whatever machinery is required to select and build an executable
 * program for the operation (e.g. an operation program manager and a cache of
 * backend-private resources), so that callers only need to provide the
 * operands and the hardware resources on which the work is to be carried out.
 *
 * Different implementations may realize different execution strategies, such
 * as eager submission or deferred graph building.
 */
class XMIPP4_CORE_API operation_dispatcher
{
public:
	operation_dispatcher() noexcept;
	operation_dispatcher(const operation_dispatcher &other) = delete;
	operation_dispatcher(operation_dispatcher &&other) = delete;
	virtual ~operation_dispatcher();

	operation_dispatcher&
	operator=(const operation_dispatcher &other) = delete;
	operation_dispatcher&
	operator=(operation_dispatcher &&other) = delete;

	/**
	 * @brief Execute an operation on the provided operands.
	 *
	 * Output operands are resolved against the operation: when they lack
	 * storage it is allocated from @p allocator, and when they already own
	 * storage it is validated and reused. The resulting command is then
	 * submitted to @p queue.
	 *
	 * @param operation The operation to be executed.
	 * @param output_operands The output operands. May be modified in place to
	 * receive freshly allocated storage and sanitized descriptors.
	 * @param input_operands The input operands. Must already own storage.
	 * @param device_context Device resources used for execution.
	 * @param queue The queue to which the operation may be executed.
	 */
	virtual void dispatch(
		const operation &operation,
		span<array> output_operands,
		span<const array_view> input_operands,
		const hardware::device_context &device
	) = 0;
};

/**
 * @brief Create a dispatcher that submits operations eagerly.
 *
 * The returned dispatcher builds and submits the executable program for each
 * operation as soon as it is dispatched, caching backend-private resources
 * (compiled kernels, FFT plans, ...) so they are reused across builds with
 * matching launch configurations.
 *
 * @param program_manager The manager queried to build the program for each
 * operation. Must not be null.
 * @return A newly created eager dispatcher.
 *
 * @throws std::invalid_argument if @p program_manager is null.
 */
XMIPP4_CORE_API
std::shared_ptr<operation_dispatcher> make_eager_operation_dispatcher(
	std::shared_ptr<const operation_program_manager> program_manager
);

} // namespace multidimensional
} // namespace xmipp4
