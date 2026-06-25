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
array floor(
    array_view x,
    const execution_context &context,
    array *out = nullptr
);

XMIPP4_CORE_API
array ceil(
    array_view x,
    const execution_context &context,
    array *out = nullptr
);

XMIPP4_CORE_API
array round(
    array_view x,
    const execution_context &context,
    array *out = nullptr
);

XMIPP4_CORE_API
array trunc(
    array_view x,
    const execution_context &context,
    array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
