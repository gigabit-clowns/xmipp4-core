// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "../service_manager.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class memory_allocator;
class memory_resource;
class memory_allocator_backend;

/**
 * @brief Centralizes and manages multiple memory_allocator_backends-s.
 * 
 */
class memory_allocator_manager
	: public service_manager
{
public:
	XMIPP4_CORE_API memory_allocator_manager() noexcept;
	memory_allocator_manager(const memory_allocator_manager &other) = delete;
	XMIPP4_CORE_API memory_allocator_manager(memory_allocator_manager &&other) noexcept;
	XMIPP4_CORE_API ~memory_allocator_manager() override;

	memory_allocator_manager& 
	operator=(const memory_allocator_manager &other) = delete;
	XMIPP4_CORE_API memory_allocator_manager& 
	operator=(memory_allocator_manager &&other) noexcept;

	XMIPP4_CORE_API 
	void register_builtin_backends() override;

	/**
	 * @brief Register a new memory_allocator_backend.
	 * 
	 * @param backend The backend to be registered.
	 * @return true The backend was successfully registered.
	 * @return false The backend could not be registered.
	 */
	XMIPP4_CORE_API 
	bool register_backend(std::unique_ptr<memory_allocator_backend> backend);

	/**
	 * @brief Create a memory allocator to allocate buffers on the provided
	 * memory_resource.
	 * 
	 * @param resource The memory resource where the allocator creates memory
	 * buffers.
	 * @return std::shared_ptr<memory_allocator> The newly created allocator.
	 */
	XMIPP4_CORE_API
	std::shared_ptr<memory_allocator> create_memory_allocator(
		memory_resource &resource
	) const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	void create_implementation_if_null();
}; 

} // namespace hardware
} // namespace xmipp4
