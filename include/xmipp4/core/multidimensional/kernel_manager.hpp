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

namespace multidimensional
{

class operation;
class array_descriptor;
class kernel_builder;
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

	XMIPP4_CORE_API
	bool register_kernel(std::unique_ptr<kernel_builder> builder);

	XMIPP4_CORE_API
	std::shared_ptr<kernel> build_kernel(
		const operation &operation,
		span<const array_descriptor> descriptors,
		hardware::device &device
	) const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	void create_if_null();
};

} // namespace multidimensional
} // namespace xmipp4
