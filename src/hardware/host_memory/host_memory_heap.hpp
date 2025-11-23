// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_heap.hpp>

namespace xmipp4 
{
namespace hardware
{

class host_memory_heap final
	: public memory_heap
{
public:
	host_memory_heap() noexcept;
	host_memory_heap(std::size_t size, std::size_t alignment);
	~host_memory_heap() override;

	std::size_t get_size() const noexcept override;

	std::shared_ptr<buffer> create_buffer(
		std::size_t offset, 
		std::size_t size,
		std::unique_ptr<buffer_sentinel> sentinel
	) override;

private:
	std::size_t m_size;
	void *m_data;
};

} // namespace hardware
} // namespace xmipp4
