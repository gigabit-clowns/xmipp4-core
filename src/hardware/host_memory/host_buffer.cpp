// SPDX-License-Identifier: GPL-3.0-only

#include "host_buffer.hpp"

#include <xmipp4/core/memory/aligned_alloc.hpp>

#include "host_memory_resource.hpp"

#include <stdexcept>

namespace xmipp4
{
namespace hardware
{

host_buffer::host_buffer(
	std::size_t size, 
	std::size_t alignment
)
	: buffer(
		memory::aligned_alloc(size, alignment), 
		size, 
		host_memory_resource::get(), 
		nullptr
	)
{
	if (size > 0 && get_host_ptr() == nullptr)
	{
		throw std::bad_alloc();
	}
}

host_buffer::~host_buffer()
{
	memory::aligned_free(get_host_ptr());
}

} // namespace hardware
} // namespace xmipp4
