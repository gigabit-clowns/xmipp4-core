// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class storage;
class kernel_access_layout;

class kernel
{
public:
    virtual ~kernel() = default;

    virtual void execute(
        const kernel_access_layout &iteration_layout,
		span<const numerical_type> data_types,
        span<storage> read_write_operands,
        span<const storage> read_only_operands
		/* TODO context */
    ) = 0;

};

} // namespace multidimensional
} // namespace xmipp4
