// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Ensure that the index is within bounds for a given extent.
 * 
 * This function ensures that the input index is within bounds for the
 * provided axis extent. In addition, it replaces negative values with
 * indices referred from the end of the array.
 * 
 * @param index Index to be checked.
 * @param extent Extent of the indexed axis.
 * @return std::size_t Sanitize value. Guaranteed to be in [0, extent)
 * @throws std::out_of_range if the provided index is out of [-extent, extent)
 */
std::size_t sanitize_index(std::ptrdiff_t index, std::size_t extent);

} // namespace multidimensional
} // namespace xmipp4
