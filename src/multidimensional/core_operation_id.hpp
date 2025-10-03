// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_id.hpp>

namespace xmipp4 
{
namespace multidimensional
{

template <typename Str>
operation_id make_core_operation_id(Str &&name);

} // namespace multidimensional
} // namespace xmipp4

#include "core_operation_id.inl"
