// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <stdexcept>

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{

/**
 * @brief Exception indicating that the function call is not available due to
 * a misconfiguration of the class.
 * 
 */
XMIPP4_STD_BASE_DLL_INTERFACE_BEGIN
class XMIPP4_CORE_API invalid_operation_error
	: public std::logic_error
{
	using logic_error::logic_error;
};
XMIPP4_STD_BASE_DLL_INTERFACE_END

} // namespace xmipp4
