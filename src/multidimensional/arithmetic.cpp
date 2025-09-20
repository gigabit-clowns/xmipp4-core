// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/arithmetic.hpp>

#include <xmipp4/core/multidimensional/context.hpp>
#include <xmipp4/core/multidimensional/allocator.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/kernel_access_layout.hpp>
#include <xmipp4/core/multidimensional/operation_id.hpp>
#include <xmipp4/core/multidimensional/operation_id_builder.hpp>
#include <xmipp4/core/multidimensional/kernel_registry.hpp>
#include <xmipp4/core/multidimensional/kernel_builder.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>

namespace xmipp4 
{
namespace multidimensional
{

const operation_id& get_add_operation_id() noexcept
{
    static const operation_id id = operation_id_builder()
        .nest("core")
        .build("add");

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
