// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation_id.hpp"

#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <ostream>
#include <string>

namespace xmipp4 
{
namespace multidimensional
{

class operation_id;
class array_descriptor;
class strided_layout;

/**
 * @brief Interface describing the validation and deduction of array shapes.
 */
class XMIPP4_CORE_API shape_policy
{
public:
	shape_policy() noexcept;
	shape_policy(const shape_policy &other) = delete;
	shape_policy(shape_policy &&other) = delete;
	virtual ~shape_policy();

	shape_policy& operator=(const shape_policy &other) = delete;
	shape_policy& operator=(shape_policy &&other) = delete;

	/**
	 * @brief Deduce the shape of the output operands. 
	 *
	 * The shape of the output operands is deduced from the input shape. In
	 * addition, input operands may be broadcasted and validated.
	 * 
	 * @param output_layouts Layouts of output operands.
	 * @param input_layouts Layouts of input operands.
	 */
	virtual void infer_output(
		span<strided_layout> output_layouts,
		span<strided_layout> input_layouts
	) const = 0;

	/**
	 * @brief Validate the shape of the operands. 
	 * 
	 * Output operands are considered as "given" and they are not modified.
	 * However, input operands may be broadcasted and validated to match the 
	 * shape requirements.
	 * 
	 * @param output_layouts Layouts of output operands.
	 * @param input_layouts Layouts of input operands.
	 */
	virtual void validate(
		span<const strided_layout> output_layouts,
		span<strided_layout> input_layouts
	) const = 0;
};

/**
 * @brief Interface describing the validation and deduction of array data types.
 */
class XMIPP4_CORE_API data_type_policy
{
public:
	data_type_policy() noexcept;
	data_type_policy(const data_type_policy &other) = delete;
	data_type_policy(data_type_policy &&other) = delete;
	virtual ~data_type_policy();

	data_type_policy& operator=(const data_type_policy &other) = delete;
	data_type_policy& operator=(data_type_policy &&other) = delete;

	/**
	 * @brief Deduce the data type of the output operands. 
	 *
	 * The data type of the output operands is deduced from the input shape. In
	 * addition input types may be validated.
	 * 
	 * @param output_types Data types of output operands.
	 * @param input_types Data types of input operands.
	 */
	virtual void infer_output(
		span<numerical_type> output_types,
		span<const numerical_type> input_types
	) const = 0;

	/**
	 * @brief Validate the data types of the input and output operands. 
	 *
	 * Validates the data types of all operands.
	 * 
	 * @param output_types Data types of output operands.
	 * @param input_types Data types of input operands.
	 */
	virtual void validate(
		span<const numerical_type> output_types,
		span<const numerical_type> input_types
	) const = 0;
};

/**
 * @brief Abstract class that describes an operation.
 * 
 * Subclasses of it may implement the specific behavior for each of the
 * operations.
 */
class XMIPP4_CORE_API operation
{
public:
	operation() noexcept;
	operation(const operation &other) = delete;
	operation(operation &&other) = delete;
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
	virtual std::string get_name() const = 0;

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
	virtual std::string serialize_parameters() const;

	/**
	 * @brief Get the shape policy for the operation.
	 * 
	 * @return const shape_policy& The shape policy.
	 */
	virtual const shape_policy& get_shape_policy() const noexcept = 0;

	/**
	 * @brief Get the data type policy for the operation.
	 * 
	 * @return const shape_policy& The data type policy.
	 */
	virtual const data_type_policy& get_data_type_policy() const noexcept = 0;
};

std::ostream& operator<<(std::ostream& os, const operation& op);

std::string to_string(const operation& op);

} // namespace multidimensional
} // namespace xmipp4
