// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

void broadcast_to(std::vector<std::size_t> &from, span<const std::size_t> to);

} // namespace multidimensional
} // namespace xmipp4
