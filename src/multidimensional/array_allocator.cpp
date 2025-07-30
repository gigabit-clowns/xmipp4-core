// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array_allocator.hpp>

#include "array_allocator_implementation.hpp"

namespace xmipp4 
{
namespace multidimensional
{

static array_allocator_implementation default_array_allocator;

array_allocator& get_default_array_allocator() noexcept
{
    return default_array_allocator;
}

} // namespace multidimensional
} // namespace xmipp4
