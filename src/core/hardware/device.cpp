// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/device.hpp>

namespace xmipp4
{
namespace hardware
{

device::device(device_properties properties) noexcept
	: m_properties(std::move(properties))
{
}

device::~device() = default;

const device_properties& device::get_properties() const noexcept
{
	return m_properties;
}

} // namespace hardware
} // namespace xmipp4
