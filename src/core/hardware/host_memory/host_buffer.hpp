// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/buffer.hpp>

namespace xmipp4 
{
namespace hardware
{

/**
 * @brief Implementation of buffer_sentinel based on malloc/free
 * 
 */
class host_buffer final
	: public buffer
{
public:
	host_buffer(std::size_t size, std::size_t alignment);
	~host_buffer() override;
};

} // namespace hardware
} // namespace xmipp4
