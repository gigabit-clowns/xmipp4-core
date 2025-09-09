// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../numerical_type.hpp"
#include "../const_any_reference.hpp"

#include <stdexcept>

namespace xmipp4 
{

namespace multidimensional
{

class operation_id;
class kernel_iteration_layout;
class storage;
class context;

void dispatch(const operation_id &key, 
              const kernel_iteration_layout &iteration_layout,
              span<numerical_type> numerical_types,
              span<storage * const> read_write_operands, 
              span<const storage * const> read_only_operands, 
              const context &context,
              const_any_reference parameters = {});

} // namespace multidimensional
} // namespace xmipp4

