// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"
#include "../numerical_type.hpp"
#include "../backend_priority.hpp"

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
class operation_id;
class strided_layout;
class kernel;

class XMIPP4_CORE_API kernel_builder
{
public:
	kernel_builder() noexcept;
	kernel_builder(const kernel_builder &other) = default;
	kernel_builder(kernel_builder &&other) = default;
    virtual ~kernel_builder();

	kernel_builder& operator=(const kernel_builder &other) = default;
	kernel_builder& operator=(kernel_builder &&other) = default;

	virtual const operation_id& get_operation_id() const noexcept = 0;

	virtual backend_priority get_suitability(
		const operation &operation,
		span<const strided_layout> layouts,
		span<const numerical_type> data_types,
		hardware::device &device
	) const = 0;

    virtual 
    std::shared_ptr<kernel> build(
		const operation &operation,
		span<const strided_layout> layouts,
		span<const numerical_type> data_types,
		hardware::device &device
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
