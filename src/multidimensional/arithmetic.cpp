// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/arithmetic.hpp>

#include <xmipp4/core/multidimensional/context.hpp>
#include <xmipp4/core/multidimensional/allocator.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/kernel_iteration_layout.hpp>
#include <xmipp4/core/multidimensional/operation_key.hpp>
#include <xmipp4/core/multidimensional/kernel_registry.hpp>
#include <xmipp4/core/multidimensional/kernel_builder.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>

namespace xmipp4 
{
namespace multidimensional
{

const operation_key& get_add_operation_key() noexcept
{
    struct tag {};
    static const operation_key key = operation_key::from_tag<tag>();
    return key;
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
