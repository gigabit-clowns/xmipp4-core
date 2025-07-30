// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array;

array& fill(array& x /* TODO */);

array& fill_zeros(array& x);

array& fill_ones(array& x);

} // namespace multidimensional
} // namespace xmipp4
