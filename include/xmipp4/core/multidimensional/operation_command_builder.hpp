// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation_id.hpp"
#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"
#include "../numerical_type.hpp"
#include "../backend_priority.hpp"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class command;

} // namespace hardware

namespace multidimensional
{

class operation;
class array_signature;
class operation_command_cache;

/**
 * @brief Abstract representation of a factory class for operation commands
 */
class operation_command_builder
{
public:
	XMIPP4_CORE_API
	operation_command_builder() noexcept;
	operation_command_builder(const operation_command_builder &other) = default;
	operation_command_builder(operation_command_builder &&other) = default;
	XMIPP4_CORE_API
	virtual ~operation_command_builder();

	operation_command_builder& 
	operator=(const operation_command_builder &other) = default;
	operation_command_builder& 
	operator=(operation_command_builder &&other) = default;

	/**
	 * @brief Get the operation identifier for which this builder is 
	 * specialized.
	 * 
	 * @return operation_id The operation ID.
	 */
	virtual operation_id get_operation_id() const noexcept = 0;

	/**
	 * @brief Get the suitability of this builder for a given launch 
	 * configuration.
	 * 
	 * @param operation The operation. Its ID must be equal to the ID returned
	 * by get_operation_id.
	 * @param output_signatures The output array signatures involved in the 
	 * operation.
	 * @param input_signatures The output array signatures involved in the 
	 * operation.
	 * @return backend_priority The suitability of this builder for the 
	 * requested launch configuration.
	 */
	virtual backend_priority get_suitability(
		const operation &operation,
		span<const array_signature> output_signatures,
		span<const array_signature> input_signatures
	) const = 0;

	/**
	 * @brief Build an executable operation command for a given launch 
	 * configuration.
	 *
	 * This method may not be called if get_suitability() returns
	 * backend_priority::unsupported.
	 *
	 * @param operation The operation. Its ID must be equal to the ID returned
	 * by get_operation_id.
	 * @param output_signatures The output array signatures involved in the
	 * operation.
	 * @param input_signatures The output array signatures involved in the
	 * operation.
	 * @param cache Optional cache for backend-private resources. When not
	 * null, the builder may consult and update it to reuse expensive
	 * resources (FFT plans, compiled kernels, workspaces, ...) across
	 * builds. Implementations are free to ignore it.
	 * @return std::shared_ptr<hardware::command> The executable command suited
	 * for the requested operation and signature.
	 */
	virtual
	std::shared_ptr<hardware::command> build(
		const operation &operation,
		span<const array_signature> output_signatures,
		span<const array_signature> input_signatures,
		operation_command_cache *cache
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
