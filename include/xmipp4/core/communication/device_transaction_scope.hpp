// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4 
{
namespace communication
{

class device_transaction;

/**
 * @brief RAII style wrapper to device_transaction::begin() and 
 * device_transaction::commit()
 * 
 */
class device_transaction_scope
{
public:
	device_transaction_scope() noexcept;
	explicit device_transaction_scope(device_transaction *transaction);
	device_transaction_scope(const device_transaction_scope &other) = delete;
	device_transaction_scope(device_transaction_scope &&other) = delete;
	~device_transaction_scope();

	device_transaction_scope& 
	operator=(const device_transaction_scope &other) = delete;
	device_transaction_scope& 
	operator=(device_transaction_scope &&other) = delete;

	void commit();

private:
	device_transaction *m_transaction;

};

} // namespace communication
} // namespace xmipp4
