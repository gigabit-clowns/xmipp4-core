// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/binary/flagset.hpp>

namespace xmipp4 
{
namespace hardware
{

/**
 * @brief Capability bits describing how an event may be used.
 *
 * Each bit represents a distinct synchronization capability that an event
 * may support. The set of bits requested at event creation lets the backend
 * pick the cheapest underlying primitive that satisfies the requested
 * capabilities.
 */
enum class event_usage_flag_bits
{
	/// The event is used to profile execution times. The timestamp when the 
	/// provided execution checkpoint is reached will be stored.
	timestamp = binary::bit(0),

	/// The event can be queried from the host thread.
	host_query = binary::bit(1),

	/// The event can be waited on from the host thread.
	host_wait = binary::bit(2),

	/// The event can be waited on another queue from the same device that 
	/// signals it.
	device_wait = binary::bit(3),

	/// The event can be waited on from a queue on a different device.
	cross_device_wait = binary::bit(4)
};

/**
 * @brief Set of @ref event_usage_flag_bits.
 */
using event_usage_flags = binary::flagset<event_usage_flag_bits>;

} // namespace hardware
} // namespace xmipp4
