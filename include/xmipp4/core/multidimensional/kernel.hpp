// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../span.hpp"

namespace xmipp4 
{
namespace hardware
{

class device_queue;
	
} // namespace hardware


namespace multidimensional
{

class array;
class operation_parameters;

class kernel
{
public:
	kernel() = default;
	kernel(const kernel &other) = delete;
	kernel(kernel &&other) = delete;
    virtual ~kernel() = default;

	kernel& operator=(const kernel &other) = delete;
	kernel& operator=(kernel &&other) = delete;

    virtual void execute(
        span<array> read_write_operands,
        span<const array> read_only_operands,
		hardware::device_queue *queue
    ) = 0;

};

} // namespace multidimensional
} // namespace xmipp4
