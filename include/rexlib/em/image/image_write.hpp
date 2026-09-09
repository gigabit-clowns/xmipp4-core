// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <rexlib/core/numerical/numerical_type.hpp>
#include <rexlib/core/platform/dynamic_shared_object.h>
#include <rexlib/core/ndarray/const_array_ref.hpp>
#include <rexlib/em/image/image_metadata.hpp>
#include <rexlib/em/image/image_write_format_manager.hpp>

#include <cstddef>

namespace rexlib
{
namespace em
{

REXLIB_API
void write(
	const_array_ref arr, 
	const std::string &path, 
	const image_write_format_manager &manager,
	numerical_type data_type = numerical_type::unknown,
	image_metadata metadata = image_metadata()
);

} // namespace em
} // namespace rexlib
