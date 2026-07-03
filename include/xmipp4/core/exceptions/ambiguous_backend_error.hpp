// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <stdexcept>

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{

/**
 * @brief Exception indicating that could not disambiguate among multiple
 * backend candidates.
 * 
 */
XMIPP4_STD_BASE_DLL_INTERFACE_BEGIN
class XMIPP4_CORE_API ambiguous_backend_error
	: public std::runtime_error
{
	using runtime_error::runtime_error;
};
XMIPP4_STD_BASE_DLL_INTERFACE_END

} // namespace xmipp4
