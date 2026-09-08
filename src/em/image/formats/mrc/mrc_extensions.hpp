// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>

namespace rexlib
{
namespace em
{
namespace mrc
{

/**
 * @brief Check whether an extension is one an MRC file is read from.
 *
 * Wider than @ref is_writable_extension: a file being read already exists and
 * is only reached by extension when it carries no identifier, whereas the
 * extension is all there is to go on when one is created.
 *
 * @param extension The extension, folded to lower case and including its
 * leading dot, as @ref image_probe reports it.
 * @return bool true if an MRC file is read from it.
 */
bool is_readable_extension(const std::string &extension) noexcept;

/**
 * @brief Check whether an extension is one an MRC file is created with.
 *
 * @param extension The extension, folded to lower case and including its
 * leading dot.
 * @return bool true if an MRC file is created with it.
 */
bool is_writable_extension(const std::string &extension) noexcept;

} // namespace mrc
} // namespace em
} // namespace rexlib
