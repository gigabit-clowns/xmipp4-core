// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../service_manager.hpp"
#include "../numerical_type.hpp"
#include "../span.hpp"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class device;

} // namespace hardware

class kernel_builder;

namespace multidimensional
{

class strided_layout;
class operation_id;
class kernel;

class kernel_manager
	: service_manager
{
public:
	XMIPP4_CORE_API
	kernel_manager() noexcept;
	kernel_manager(const kernel_manager &other) = delete;
	kernel_manager(kernel_manager &&other) = delete;
	XMIPP4_CORE_API
	~kernel_manager() override;

	kernel_manager& operator=(const kernel_manager &other) = delete;
	kernel_manager& operator=(kernel_manager &&other) = delete;

	XMIPP4_CORE_API
	void register_builtin_backends() override;

	bool register_builder(std::unique_ptr<kernel_builder> builder);

	std::shared_ptr<kernel> build_kernel(
		span<const strided_layout> layouts,
		span<const numerical_type> data_types,
		/* TODO operation parameters */
		hardware::device &device
	);

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	void create_if_null();

};

} // namespace multidimensional
} // namespace xmipp4
