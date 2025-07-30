// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array_operations.hpp>

#include "array_point_operation.hpp"

namespace xmipp4 
{
namespace multidimensional
{

array& fill_zeros(array& x)
{
    return point_operation<0>()(
        {},
        []() -> auto { return 0; }, 
        x
    );
}

array& fill_ones(array& x)
{
    return point_operation<0>()(
        {},
        []() -> auto { return 1; }, 
        x
    );
}

array& copy(const array& from, array& to)
{
    return point_operation<1>()(
        {from},
        [](const auto &x) -> auto { return x; }, 
        to
    );
}

} // namespace multidimensional
} // namespace xmipp4
