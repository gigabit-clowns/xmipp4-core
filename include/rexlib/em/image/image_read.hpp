// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <rexlib/core/numerical/numerical_type.hpp>
#include <rexlib/core/platform/dynamic_shared_object.h>
#include <rexlib/core/ndarray/array.hpp>
#include <rexlib/core/dispatch/execution_context.hpp>
#include <rexlib/em/image/image_metadata.hpp>
#include <rexlib/em/image/image_location.hpp>
#include <rexlib/em/image/image_read_format_manager.hpp>

#include <cstddef>

namespace rexlib
{
namespace em
{

REXLIB_API
array read(
	const std::string &path, 
	const image_read_format_manager &manager,
	const execution_context &context
);

REXLIB_API
array read(
	const image_location &location, 
	const image_read_format_manager &manager,
	const execution_context &context
);

} // namespace em
} // namespace rexlib
