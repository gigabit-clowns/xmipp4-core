// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "context.hpp"
#include "../numerical_type.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class array;

array& fill(array& x /* TODO */);

array& fill_zeros(array& x, const context &context = get_default_context());

array& fill_ones(array& x, const context &context = get_default_context());

array& copy(const array& from, array& to, const context &context = get_default_context());

} // namespace multidimensional
} // namespace xmipp4
