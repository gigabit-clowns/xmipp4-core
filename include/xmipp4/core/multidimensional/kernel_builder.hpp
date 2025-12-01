// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../numerical_type.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array_access_layout;
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
		const array_access_layout &access_layout,
		span<const numerical_type> data_types
    	/*TODO parameters*/ 
    	/*TODO compute context*/ 
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
