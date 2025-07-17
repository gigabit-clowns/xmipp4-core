// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file core_version.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of core_version.hpp
 * @date 2024-03-03
 * 
 */

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
