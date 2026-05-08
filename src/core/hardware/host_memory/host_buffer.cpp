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
	: m_data(memory::aligned_alloc(size, alignment))
	, m_size(size)
{
	if (size > 0 && m_data == nullptr)
	{
		throw std::bad_alloc();
	}
}

host_buffer::~host_buffer()
{
	memory::aligned_free(get_host_ptr());
}


void* host_buffer::get_host_ptr() noexcept
{
	return m_data;
}

const void* host_buffer::get_host_ptr() const noexcept
{
	return m_data;
}

void* host_buffer::get_device_handle() noexcept
{
	return nullptr;
}

const void* host_buffer::get_device_handle() const noexcept
{
	return nullptr;
}

std::size_t host_buffer::get_size() const noexcept
{
	return m_size;
}

const memory_resource& host_buffer::get_memory_resource() const noexcept
{
	return get_host_memory_resource();
}

} // namespace hardware
} // namespace xmipp4
