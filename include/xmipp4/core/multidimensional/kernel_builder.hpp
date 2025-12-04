// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../numerical_type.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class operation_parameters;
class strided_layout;
class kernel;

class kernel_builder
{
public:
	kernel_builder() = default;
	kernel_builder(const kernel_builder &other) = default;
	kernel_builder(kernel_builder &&other) = default;
    virtual ~kernel_builder() = default;

	kernel_builder& operator=(const kernel_builder &other) = default;
	kernel_builder& operator=(kernel_builder &&other) = default;

    virtual 
    std::shared_ptr<kernel> build(
		span<const strided_layout> layouts,
		span<const numerical_type> data_types,
		const operation_parameters *parameters
		/* TODO hardware context */
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
