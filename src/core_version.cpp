// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/core_version.hpp>

namespace xmipp4
{

version get_core_version() noexcept
{
    return version(
        VERSION_MAJOR,
        VERSION_MINOR,
        VERSION_PATCH
    );
}

} // namespace xmipp4
