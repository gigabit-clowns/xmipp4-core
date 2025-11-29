// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/device_transaction.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace communication
{

class mock_device_transaction final
	: public device_transaction
{
public:
	MAKE_MOCK0(begin, void(), override);
	MAKE_MOCK0(commit, void(), override);
};

} // namespace communication
} // namespace xmipp4
