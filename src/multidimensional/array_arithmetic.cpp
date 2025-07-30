// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array_arithmetic.hpp>

#include "array_point_operation.hpp"

#include <functional>

namespace xmipp4 
{
namespace multidimensional
{

array& add(const array& lhs, const array& rhs, array &output)
{
    return point_operation<2>()({lhs, rhs}, std::plus<>(), output);
}

std::shared_ptr<array> 
add(const array& lhs, const array& rhs, array_allocator &allocator)
{
    return point_operation<2>()({lhs, rhs}, std::plus<>(), allocator);
}

array& subtract(const array& lhs, const array& rhs, array &output)
{
    return point_operation<2>()({lhs, rhs}, std::minus<>(), output);
}

std::shared_ptr<array> 
subtract(const array& lhs, const array& rhs, array_allocator &allocator)
{
    return point_operation<2>()({lhs, rhs}, std::minus<>(), allocator);
}

array& multiply(const array& lhs, const array& rhs, array &output)
{
    return point_operation<2>()({lhs, rhs}, std::multiplies<>(), output);
}

std::shared_ptr<array> 
multiply(const array& lhs, const array& rhs, array_allocator &allocator)
{
    return point_operation<2>()({lhs, rhs}, std::multiplies<>(), allocator);
}

array& divide(const array& lhs, const array& rhs, array &output)
{

    return point_operation<2>()({lhs, rhs}, std::divides<>(), output);
}

std::shared_ptr<array> 
divide(const array& lhs, const array& rhs, array_allocator &allocator)
{

    return point_operation<2>()({lhs, rhs}, std::divides<>(), allocator);
}

} // namespace multidimensional
} // namespace xmipp4
