// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file ambiguous_backend_exception.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of ambiguous_backend_error
 * @date 2024-11-20
 * 
 */

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

