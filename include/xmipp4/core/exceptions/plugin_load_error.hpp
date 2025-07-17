// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file plugin_load_error.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of plugin_load_error
 * @date 2024-11-21
 * 
 */

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
