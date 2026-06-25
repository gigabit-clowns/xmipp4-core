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
array matmul(
    array_view a,
    array_view b,
    const execution_context &context,
    array *out = nullptr
);

XMIPP4_CORE_API
array matvec(
    array_view a,
    array_view x,
    const execution_context &context,
    array *out = nullptr
);

XMIPP4_CORE_API
array dot(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out = nullptr
);

XMIPP4_CORE_API
array outer(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
