// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class storage;
class array_access_layout;
class operation_parameters;

class kernel
{
public:
	kernel() = default;
	kernel(const kernel &other) = delete;
	kernel(kernel &&other) = delete;
    virtual ~kernel() = default;

	kernel& operator=(const kernel &other) = delete;
	kernel& operator=(kernel &&other) = delete;

    virtual void execute(
        const array_access_layout &iteration_layout,
		span<const numerical_type> data_types,
		const operation_parameters *parameters,
        span<storage> read_write_operands,
        span<const storage> read_only_operands
		/* TODO hardware context */
    ) = 0;

};

} // namespace multidimensional
} // namespace xmipp4
