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
	/**
	 * @brief Construct a new transaction scope without initiating it.
	 * 
	 */
	device_transaction_scope() noexcept;

	/**
	 * @brief Construct a device transaction scope optionally initiating it. 
	 * 
	 * @param transaction The transaction used in the scope. May be null
	 * in which case this function behaves as the default constructor. 
	 */
	explicit device_transaction_scope(device_transaction *transaction);

	/**
	 * @brief Construct a device transaction scope by initiating it.
	 * 
	 * @param transaction The transaction used in the scope.
	 */
	explicit device_transaction_scope(device_transaction &transaction);

	device_transaction_scope(const device_transaction_scope &other) = delete;
	device_transaction_scope(device_transaction_scope &&other) = delete;

	/**
	 * @brief Destroy the device transaction.
	 * 
	 * If the scope was not concluded, the destructor calls commit().
	 * 
	 */
	~device_transaction_scope();

	device_transaction_scope& 
	operator=(const device_transaction_scope &other) = delete;
	device_transaction_scope& 
	operator=(device_transaction_scope &&other) = delete;

	/**
	 * @brief Conclude this transaction scope. 
	 * 
	 * If it was not initiated, this method is a no-op. If a transaction 
	 * scope was initiated and this method is not called, it will be called
	 * in the destructor. 
	 * 
	 */
	void commit();

private:
	device_transaction *m_transaction;
};

} // namespace communication
} // namespace xmipp4
