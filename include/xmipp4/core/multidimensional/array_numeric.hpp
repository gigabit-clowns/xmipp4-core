// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "array_view.hpp"

namespace xmipp4
{
namespace multidimensional
{

class execution_context;

XMIPP4_CORE_API
array minimum(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out = nullptr
);

XMIPP4_CORE_API
array maximum(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out = nullptr
);

XMIPP4_CORE_API
array clamp(
    array_view x,
    array_view lo,
    array_view hi,
    const execution_context &context,
    array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
