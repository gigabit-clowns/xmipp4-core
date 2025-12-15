// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation_id.hpp"
#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"

#include <string>

namespace xmipp4 
{
namespace multidimensional
{

class operation_id;
class array_descriptor;

/**
 * @brief Abstract class that describes an operation.
 * 
 * Subclasses of it may implement the specific behavior for each of the
 * operations.
 * 
 */
class operation
{
public:
	XMIPP4_CORE_API
	operation() noexcept;
	operation(const operation &other) = delete;
	operation(operation &&other) = delete;
	XMIPP4_CORE_API
    virtual ~operation();

	operation& operator=(const operation &other) = delete;
	operation& operator=(operation &&other) = delete;

	/**
	 * @brief Get an identifier that uniquely represents this operation.
	 * 
	 * @return const operation_id& The operation id.
	 */
	operation_id get_id() const noexcept;

	/**
	 * @brief Get a human readable identifier of the operation.
	 * 
	 * This representation should not encode the parameters of the operation,
	 * i.e., it should be the same for all instances of a given operation class.
	 * 
	 * @return std::string The human readable representation.
	 */
	virtual std::string get_name() const noexcept = 0;

	/**
	 * @brief Serialize the parameters of the operation.
	 * 
	 * Obtain a string representation of the parameters of this operation. The
	 * actual representation is implementation dependant. The only requirement
	 * is that unequal parameters should have unequal serializations. The 
	 * serialization does not need to encode the operation_id nor the name.
	 * 
	 * @return std::string String representation of the operation parameters.
	 */
	XMIPP4_CORE_API
	virtual std::string serialize_parameters() const;

	/**
	 * @brief Process and validate the input output operands
	 * 
	 * This method validate and adapt operands to perform the represented 
	 * operations. Depending on the operation type and descriptors, calling
	 * this function may:
	 * 
	 * - Validate invariance in the input (e.g. inner matrix dimensions in a
	 * matrix multiplication).
	 * - Deduce and populate output operand's descriptors when default 
	 * constructed ones are provided.
	 * - Perform broadcasting in input operands.
	 * 
	 * @param output_descriptors The output descriptors.
	 * @param input_descriptors The input operands.
	 */
	virtual void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
