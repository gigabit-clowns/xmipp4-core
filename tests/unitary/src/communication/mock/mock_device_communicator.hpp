// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/device_communicator.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace communication
{

class mock_device_communicator final
	: public device_communicator
{
public:
	MAKE_CONST_MOCK0(get_size, std::size_t(), override);
	MAKE_CONST_MOCK0(get_rank, std::size_t(), override);
	MAKE_CONST_MOCK0(get_device, hardware::device&(), noexcept override);
	MAKE_CONST_MOCK2(
		split, 
		std::shared_ptr<device_communicator>(int, int), 
		override
	);
	MAKE_MOCK1(barrier, void(hardware::device_queue*), override);

};

} // namespace communication
} // namespace xmipp4
