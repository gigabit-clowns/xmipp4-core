// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation_id.hpp"
#include "operation_shape_policy.hpp"
#include "operation_data_type_policy.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <ostream>
#include <string>

namespace xmipp4
{
namespace multidimensional
{

class operation_id;

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
	virtual 
	std::string get_name() const = 0;

	/**
	 * @brief Get the output count of the operation.
	 * 
	 * @return std::size_t The output count.
	 */
	virtual 
	std::size_t get_output_count() const noexcept = 0;

	/**
	 * @brief Get the input count of the operation.
	 * 
	 * @return std::size_t The input count.
	 */
	virtual 
	std::size_t get_input_count() const noexcept = 0;

	/**
	 * @brief Get the shape policy for the operation.
	 *
	 * @return const operation_shape_policy& The shape policy.
	 */
	virtual 
	const operation_shape_policy& 
	get_operation_shape_policy() const noexcept = 0;

	/**
	 * @brief Get the data type policy for the operation.
	 *
	 * @return const operation_data_type_policy& The data type policy.
	 */
	virtual 
	const operation_data_type_policy& 
	get_operation_data_type_policy() const noexcept = 0;
};

} // namespace multidimensional
} // namespace xmipp4
