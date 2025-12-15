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

class device;

} // namespace hardware

namespace multidimensional
{

class kernel;
class operation;
class array_descriptor;

/**
 * @brief Abstract representation of a factory class for kernels
 * 
 */
class kernel_builder
{
public:
	XMIPP4_CORE_API
	kernel_builder() noexcept;
	kernel_builder(const kernel_builder &other) = default;
	kernel_builder(kernel_builder &&other) = default;
	XMIPP4_CORE_API
	virtual ~kernel_builder();

	kernel_builder& operator=(const kernel_builder &other) = default;
	kernel_builder& operator=(kernel_builder &&other) = default;

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
	 * @param descriptors The array descriptors involved in the operation.
	 * @param device The device where the operation is expected to be executed.
	 * @return backend_priority The suitability of this builder for the 
	 * requested launch configuration.
	 */
	virtual backend_priority get_suitability(
		const operation &operation,
		span<const array_descriptor> descriptors,
		hardware::device &device
	) const = 0;

	/**
	 * @brief Build an executable kernel for a given launch configuration.
	 * 
	 * This method may not be called if get_suitability() returns
	 * backend_priority::unsupported.
	 * 
	 * @param operation The operation. Its ID must be equal to the ID returned
	 * by get_operation_id.
	 * @param descriptors The array descriptors involved in the operation.
	 * @param device The device where the operation is expected to be executed.
	 * @return std::shared_ptr<kernel> The executable kernel suited for the
	 * requested launch parameters.
	 */
	virtual 
	std::shared_ptr<kernel> build(
		const operation &operation,
		span<const array_descriptor> descriptors,
		hardware::device &device
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
