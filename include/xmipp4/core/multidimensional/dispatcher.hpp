// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../numerical_type.hpp"
#include "../span.hpp"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class device;
class device_queue;
	
} // namespace hardware

namespace multidimensional
{

class operation;
class array;

class dispatcher
{
public:

	void dispatch(
		const operation &operation,
		span<array> read_write_operands,
		span<const array> read_only_operands,
		hardware::device &device,
		hardware::device_queue *queue
	);

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

};

} // namespace multidimensional
} // namespace xmipp4
