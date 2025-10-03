// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/arithmetic.hpp>

#include <xmipp4/core/multidimensional/operation.hpp>

#include "core_operation_id.hpp"

namespace xmipp4 
{
namespace multidimensional
{

const operation_id& get_add_operation_id() noexcept
{
    static const auto id = make_core_operation_id("add"); 
    return id;
}

array add(const array &a, const array &b, const context &ctx, array *out)
{

}

array multiply(const array &a, const array &b, const context &ctx, array *out)
{
    // TODO similar to add changing key
}

} // namespace multidimensional
} // namespace xmipp4
