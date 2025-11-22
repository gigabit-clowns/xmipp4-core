// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <stdexcept>

namespace xmipp4 
{

/**
 * @brief Exception indicating that the function call is not available due to
 * a misconfiguration of the class.
 * 
 */
class invalid_operation_error
	: public std::logic_error
{
	using logic_error::logic_error;
};

} // namespace xmipp4
