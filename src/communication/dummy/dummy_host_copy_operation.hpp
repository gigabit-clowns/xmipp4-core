// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/host_operation.hpp>

#include <cstddef>

namespace xmipp4 
{
namespace communication
{

/**
 * @brief Implementation of a host_operation that copies data between send 
 * and receive buffers.
 * 
 * The dummy host_communicator may require dispatching an operation
 * which copies data from the send buffer to the receive buffer.
 * This class can be used to perform this task.
 * 
 */
class dummy_host_copy_operation final
	: public host_operation
{
public:
	explicit dummy_host_copy_operation(
		const void *source,
		void *destination,
		std::size_t byte_count
	) noexcept;

	~dummy_host_copy_operation() override = default;

	void execute() override;

private:
	const void *m_source;
	void *m_destination;
	std::size_t m_byte_count;

};

} // namespace communication
} // namespace xmipp4
