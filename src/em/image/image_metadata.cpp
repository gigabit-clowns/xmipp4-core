// SPDX-License-Identifier: GPL-3.0-only

#include <rexlib/em/image/image_metadata.hpp>

namespace rexlib
{
namespace em
{

image_metadata::image_metadata() noexcept = default;

image_metadata::image_metadata(const image_metadata &other) = default;
image_metadata::image_metadata(image_metadata &&other) noexcept = default;
image_metadata::~image_metadata() = default;

image_metadata&
image_metadata::operator=(const image_metadata &other) = default;
image_metadata&
image_metadata::operator=(image_metadata &&other) noexcept = default;

} // namespace em
} // namespace rexlib
