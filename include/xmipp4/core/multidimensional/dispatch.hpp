// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../const_any_reference.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class array;
class operation_key;
class context;
class operation_schema;

void dispatch(const operation_key &key, 
              const operation_schema &schema,
              span<array> outputs, 
              span<const array> inputs, 
              const context &ctx,
              const_any_reference params = {});

} // namespace multidimensional
} // namespace xmipp4

