// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation_id.hpp"
#include "shape_policy.hpp"
#include "data_type_policy.hpp"

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
	 * @return const data_type_policy& The data type policy.
	 */
	virtual const data_type_policy& get_data_type_policy() const noexcept = 0;
};

std::ostream& operator<<(std::ostream& os, const operation& op);

std::string to_string(const operation& op);

} // namespace multidimensional
} // namespace xmipp4
