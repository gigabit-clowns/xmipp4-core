// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <stdexcept>

namespace xmipp4 
{

/**
 * @brief Exception indicating that an error ocurred when loading 
 * a plugin.
 * 
 */
class plugin_load_error
	: public std::runtime_error
{
	using runtime_error::runtime_error;
};

} // namespace xmipp4
