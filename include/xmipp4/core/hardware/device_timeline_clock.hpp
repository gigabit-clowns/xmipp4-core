// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <chrono>
#include <cstdint>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief Tag clock for points on a device's execution timeline.
 *
 * The clock represents the timeline shared by every queue of a single device.
 * Its epoch is backend- and device-defined but stable for the lifetime of the
 * process, and the clock is monotonic with respect to that timeline.
 *
 * Time points obtained from events recorded on the same device, whether on
 * the same queue or on different queues, may be subtracted to yield a
 * meaningful elapsed time. Time points from events recorded on different
 * devices, or from different backends, are not directly comparable;
 * subtracting them yields an unspecified duration.
 *
 * No @c now() is provided: time points originate from queue execution rather
 * than host sampling. The host's view of the device clock is exposed by some
 * backends but not all, and is therefore not part of this abstraction.
 */
struct device_timeline_clock
{
	using rep = std::int64_t;
	using period = std::nano;
	using duration = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<device_timeline_clock, duration>;
	static constexpr bool is_steady = true;
};

} // namespace hardware
} // namespace xmipp4
