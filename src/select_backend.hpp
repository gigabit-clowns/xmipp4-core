// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4
{

template <typename C, typename ...Args>
void select_backend(const C &container, Args&& ...args);

} // namespace xmipp4
