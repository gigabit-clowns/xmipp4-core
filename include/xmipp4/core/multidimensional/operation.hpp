// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace multidimensional
{

class operation_id;

/**
 * @brief Abstract class that describes an operation.
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
	virtual const operation_id& get_operation_id() const noexcept = 0;

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
	 * @brief Broadcast input operands and infer the ouput operands if 
	 * necessary.
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
