// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_resource.hpp"

#include <xmipp4/core/system/host.hpp>

#include "host_memory_heap.hpp"

namespace xmipp4
{
namespace hardware
{

host_memory_resource host_memory_resource::m_instance;

host_memory_resource::host_memory_resource()
	: m_max_alignment(system::get_page_size())
{
}

device* host_memory_resource::get_target_device() const noexcept
{
	return nullptr;
}

memory_resource_kind host_memory_resource::get_kind() const noexcept
{
	return memory_resource_kind::host;
}

std::size_t host_memory_resource::get_max_heap_alignment() const noexcept
{
	return m_max_alignment;
}

std::shared_ptr<memory_heap> 
host_memory_resource::create_memory_heap(
	std::size_t size, 
	std::size_t alignment
)
{
	return std::make_shared<host_memory_heap>(size, alignment);
}

host_memory_resource& host_memory_resource::get() noexcept
{
	return m_instance;
}

} // namespace hardware
} // namespace xmipp4
