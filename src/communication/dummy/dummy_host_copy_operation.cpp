// SPDX-License-Identifier: GPL-3.0-only

#include "dummy_host_copy_operation.hpp"

#include <cstring>

namespace xmipp4 
{
namespace communication
{

dummy_host_copy_operation::dummy_host_copy_operation(
	const void *source,
	void *destination,
	std::size_t byte_count
) noexcept
	: m_source(source)
	, m_destination(destination)
	, m_byte_count(byte_count)
{
}

void dummy_host_copy_operation::execute()
{
	if (m_source && m_destination && m_byte_count)
	{
		std::memcpy(m_destination, m_source, m_byte_count);
	}
}

} // namespace communication
} // namespace xmipp4
