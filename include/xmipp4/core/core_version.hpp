// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file core_version.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Utilities for getting xmipp4-core version
 * @date 2024-03-03
 * 
 */

#include "version.hpp"
#include "platform/dynamic_shared_object.h"

namespace xmipp4 
{

/**
 * @brief Get the version of the xmipp-core installation
 * 
 * This function returns the version of the loaded xmipp-core .so
 * file
 * 
 * @return version Version of the installation
 */
XMIPP4_CORE_API
version get_core_version() noexcept;

} // namespace xmipp4
