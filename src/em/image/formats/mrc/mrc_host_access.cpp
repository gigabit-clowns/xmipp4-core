// SPDX-License-Identifier: GPL-3.0-only

#include "mrc_host_access.hpp"

#include <rexlib/core/exceptions/invalid_operation_error.hpp>
#include <rexlib/core/hardware/buffer.hpp>
#include <rexlib/core/hardware/memory_resource.hpp>
#include <rexlib/core/ndarray/array_ref.hpp>

#include <stdexcept>

namespace rexlib
{
namespace em
{
namespace mrc
{

void* get_host_data(array_ref array)
{
	auto *storage = array.get_storage();
	if (storage == nullptr)
	{
		throw std::invalid_argument(
			"mrc: The array is not initialized."
		);
	}

	if (!is_host_accessible(storage->get_memory_resource().get_kind()))
	{
		throw invalid_operation_error(
			"mrc: The storage of the array can not be reached from the "
			"host."
		);
	}

	auto *data = storage->get_host_ptr();
	if (data == nullptr)
	{
		throw invalid_operation_error(
			"mrc: The array does not expose its storage to the host."
		);
	}

	return data;
}

} // namespace mrc
} // namespace em
} // namespace rexlib
