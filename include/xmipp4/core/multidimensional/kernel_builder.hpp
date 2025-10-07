// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../numerical_type.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array_access_layout;
class storage;
class context;
class kernel;

class kernel_builder
{
public:
    virtual ~kernel_builder() = default;

    virtual 
    std::shared_ptr<kernel> 
    build(const kernel_access_layout &iteration_layout,
          const context &context
          /*TODO parameters*/ ) const = 0;

};

} // namespace multidimensional
} // namespace xmipp4
