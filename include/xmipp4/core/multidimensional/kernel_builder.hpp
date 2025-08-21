// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../numerical_type.hpp"
#include "../const_any_reference.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_iteration_layout;
class storage;
class context;
class kernel;

class kernel_builder
{
public:
    virtual ~kernel_builder() = default;

    virtual 
    std::unique_ptr<kernel> 
    build(const kernel_iteration_layout &iteration_layout,
          span<const numerical_type> numerical_types,
          const context &context,
          const const_any_reference &parameters = {} ) const = 0;

};

} // namespace multidimensional
} // namespace xmipp4
