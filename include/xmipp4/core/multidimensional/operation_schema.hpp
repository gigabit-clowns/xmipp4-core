// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class array_descriptor;

/**
 * @brief Abstract class to represent the signature of an operation.
 * 
 * The signature of the operation refers to the number of operands,
 * the possible invariants between operands, broadcasting rules...
 * 
 */
class operation_schema
{
public:
	XMIPP4_CORE_API
	operation_schema() noexcept;
	operation_schema(const operation_schema &other) = delete;
	operation_schema(operation_schema &&other) = delete;
	XMIPP4_CORE_API
    virtual ~operation_schema();

	operation_schema& operator=(const operation_schema &other) = delete;
	operation_schema& operator=(operation_schema &&other) = delete;

	/**
	 * @brief Perform pre-flight checks on the input operands.
	 * 
	 * This may include checks on:
	 * - Arity (number of operands).
	 * - Minimum numer of dimensions for each operand.
	 * - Possible invariants between operands (e.g. middle dimension in matrix
	 * multiplication).
	 * 
	 * @param input_descriptors The input descriptors to be checked.
	 */
	virtual void validate_input(
		span<const array_descriptor> input_descriptors
	) const = 0;
	
	/**
	 * @brief Broadcast input operands and infer the ouput operands.
	 * 
	 * @param output_descriptors The output descriptors to be inferred.
	 * @param input_descriptors The input operands that may be broadcasted.
	 */
	virtual void deduce_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
