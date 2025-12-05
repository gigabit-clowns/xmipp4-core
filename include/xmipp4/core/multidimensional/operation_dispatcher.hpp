// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class device;
class device_queue;
class memory_allocator;

} // namespace hardware

namespace multidimensional
{

class operation;
class array;
class kernel_manager;

class operation_dispatcher
{
public:
	XMIPP4_CORE_API
	operation_dispatcher() noexcept;
	operation_dispatcher(const operation_dispatcher &other) = delete;
	operation_dispatcher(operation_dispatcher &&other) = delete;
	XMIPP4_CORE_API
	~operation_dispatcher();

	operation_dispatcher& operator=(const operation_dispatcher &other) = delete;
	operation_dispatcher& operator=(operation_dispatcher &&other) = delete;

	XMIPP4_CORE_API
	void dispatch(
		const kernel_manager &manager,
		const operation &operation,
		span<array> output_operands,
		span<const array> input_operands,
		hardware::memory_allocator &allocator,
		hardware::device &device,
		hardware::device_queue *queue
	);

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

};

} // namespace multidimensional
} // namespace xmipp4
