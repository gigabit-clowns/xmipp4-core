// SPDX-License-Identifier: GPL-3.0-only

#include "core_operation_id.hpp"

#include <xmipp4/core/multidimensional/operation_id_builder.hpp>

namespace xmipp4 
{
namespace multidimensional
{

template <typename Str>
inline
operation_id make_core_operation_id(Str &&name)
{
    return operation_id_builder()
        .nest("core")
        .build(std::forward<Str>(name));
}

} // namespace multidimensional
} // namespace xmipp4

