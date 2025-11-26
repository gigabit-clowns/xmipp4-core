// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/communication/device_transaction_scope.hpp>

#include <xmipp4/core/communication/device_transaction.hpp>

namespace xmipp4
{
namespace communication
{

device_transaction_scope::device_transaction_scope() noexcept
	: m_transaction(nullptr)
{
}
device_transaction_scope::device_transaction_scope(
	device_transaction *transaction
)
	: m_transaction(transaction)
{
	if (m_transaction)
	{
		m_transaction->begin();
	}
}

device_transaction_scope::~device_transaction_scope()
{
	commit();
}

void device_transaction_scope::commit()
{
	if (m_transaction)
	{
		m_transaction->commit();
		m_transaction = nullptr;
	}
}

} // namespace communication
} // namespace xmipp4
