// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <stdexcept>

namespace xmipp4 
{

/**
 * @brief Exception indicating that could not disambiguate among multiple
 * backend candidates.
 * 
 */
class ambiguous_backend_error
    : public std::runtime_error
{
    using runtime_error::runtime_error;
};

} // namespace xmipp4

