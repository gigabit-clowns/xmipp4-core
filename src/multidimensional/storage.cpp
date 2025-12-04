// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/storage.hpp>

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

namespace xmipp4 
{
namespace multidimensional
{

storage::storage() noexcept = default;
storage::storage(storage &&other) noexcept = default;
storage::~storage() = default;
storage& storage::operator=(storage &&other) noexcept = default;

storage::storage(std::shared_ptr<hardware::buffer> buffer) noexcept
	: m_buffer(std::move(buffer))
{
}

storage storage::view() noexcept
{
	return storage(m_buffer);
}

hardware::buffer* storage::get_buffer() noexcept
{
	return m_buffer.get();
}

const hardware::buffer* storage::get_buffer() const noexcept
{
	return m_buffer.get();
}

std::size_t storage::get_size() const noexcept
{
	if (!m_buffer)
	{
		return 0UL;
	}

	return m_buffer->get_size();
}

hardware::memory_resource& storage::get_memory_resource() const noexcept
{
	if (!m_buffer)
	{
		return hardware::get_host_memory_resource(); // Placeholder
	}

	return m_buffer->get_memory_resource();
}

void storage::record_queue(hardware::device_queue &queue, bool exclusive)
{
	if (!m_buffer)
	{
		throw invalid_operation_error(
			"Can not record a queue into an empty storage"
		);
	}

	m_buffer->record_queue(queue, exclusive);
}

} // namespace multidimensional
} // namespace xmipp4
