// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class storage;
class context;
class kernel_access_layout;

class kernel
{
public:
    virtual ~kernel() = default;

    virtual void launch(const kernel_access_layout &iteration_layout,
                        span<storage *const> read_write_operands,
                        span<const storage *const> read_only_operands, 
                        const context &context ) = 0;

};

} // namespace multidimensional
} // namespace xmipp4
