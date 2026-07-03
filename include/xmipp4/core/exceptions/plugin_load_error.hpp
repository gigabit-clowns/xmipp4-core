// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <stdexcept>

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{

/**
 * @brief Exception indicating that an error ocurred when loading 
 * a plugin.
 * 
 */
XMIPP4_STD_BASE_DLL_INTERFACE_BEGIN
class XMIPP4_CORE_API plugin_load_error
	: public std::runtime_error
{
	using runtime_error::runtime_error;
};
XMIPP4_STD_BASE_DLL_INTERFACE_END

} // namespace xmipp4
