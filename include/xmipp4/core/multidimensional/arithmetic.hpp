// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "../span.hpp"

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

class context;
class operation_id;

const operation_id& get_add_operation_id() noexcept;

array add(const array &a, 
          const array &b, 
          const context &ctx, 
          array *out = nullptr );

array multiply(const array &a, 
               const array &b, 
               const context &ctx, 
               array *out = nullptr );

} // namespace multidimensional
} // namespace xmipp4

