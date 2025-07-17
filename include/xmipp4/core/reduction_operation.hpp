// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "platform/constexpr.hpp"

#include <cstddef>
#include <string_view>

namespace xmipp4
{

/**
 * @brief Operation to be performed when reducing a vector into a scalar
 * value.
 * 
 */
enum class reduction_operation
{
    sum, ///< Add all the elements of the vector.
    product, ///< Multiply all the elements of the vector.
    min, ///< Keep the smallest element of the vector.
    max ///< Keep the largest element of the vector.
};


XMIPP4_CONSTEXPR const char* to_string(reduction_operation op) noexcept;
bool from_string(std::string_view str, reduction_operation& op) noexcept;

} // namespace xmipp4

#include "reduction_operation.inl"
