// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/backend_priority.hpp>

namespace xmipp4
{

template <typename ForwardIte, typename F>
ForwardIte find_most_suitable_backend(
    ForwardIte first, 
    ForwardIte last, 
    const F& suitability_evaluator
);

} // namespace xmipp4

#include "find_most_suitable_backend.inl"
