// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class storage;
class context;

class kernel
{
public:
    virtual ~kernel() = default;

    virtual void launch(span<storage *const> outputs,
                        span<const storage *const> inputs, 
                        const context &context ) = 0;

};

} // namespace multidimensional
} // namespace xmipp4
