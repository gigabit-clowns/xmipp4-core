// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/host_communicator.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace communication
{

class mock_host_communicator final
	: public host_communicator
{
public:
	MAKE_CONST_MOCK0(get_size, std::size_t(), override);
	MAKE_CONST_MOCK0(get_rank, std::size_t(), override);
	MAKE_CONST_MOCK2(
		split, 
		std::shared_ptr<host_communicator>(int, int), 
		override
	);
	MAKE_MOCK0(barrier, void(), override);

};

} // namespace communication
} // namespace xmipp4
