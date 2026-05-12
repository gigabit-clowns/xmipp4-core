// SPDX-License-Identifier: GPL-3.0-only

#include "xmipp4/core/hardware/device_index.hpp"

#include <algorithm>
#include <charconv>

namespace xmipp4
{
namespace hardware
{

device_index::device_index(const std::string &backend_name, std::size_t device_id)
	: m_backend_name(backend_name)
	, m_device_id(device_id)
{
}

device_index::device_index(std::string &&backend_name, std::size_t device_id)
	: m_backend_name(std::move(backend_name))
	, m_device_id(device_id)
{
}

const std::string& device_index::get_backend_name() const noexcept
{
	return m_backend_name;
}

std::size_t device_index::get_device_id() const noexcept
{
	return m_device_id;
}

bool operator==(const device_index &lhs, const device_index &rhs) noexcept
{
	return lhs.get_device_id() == rhs.get_device_id() &&
		lhs.get_backend_name() == rhs.get_backend_name();
}

bool operator!=(const device_index &lhs, const device_index &rhs) noexcept
{
	return lhs.get_device_id() != rhs.get_device_id() ||
		lhs.get_backend_name() != rhs.get_backend_name();
}

bool operator<(const device_index &lhs, const device_index &rhs) noexcept
{
	if (lhs.get_backend_name() < rhs.get_backend_name())
	{
		return true;
	}
	else if(lhs.get_backend_name() == rhs.get_backend_name())
	{
		return lhs.get_device_id() < rhs.get_device_id();
	}
	return false;
}

bool operator<=(const device_index &lhs, const device_index &rhs) noexcept
{
	if (lhs.get_backend_name() < rhs.get_backend_name())
	{
		return true;
	}
	else if(lhs.get_backend_name() == rhs.get_backend_name())
	{
		return lhs.get_device_id() <= rhs.get_device_id();
	}
	return false;
}

bool operator>(const device_index &lhs, const device_index &rhs) noexcept
{
	return rhs < lhs;
}

bool operator>=(const device_index &lhs, const device_index &rhs) noexcept
{
	return rhs <= lhs;
}

std::ostream& operator<<(std::ostream &os, const device_index &index)
{
	return os << index.get_backend_name() << ':' << index.get_device_id();
}

bool parse_device_index(std::string_view text, device_index &result)
{
	if(text.empty())
	{
		return false;
	}

	const char separator = ':';
	bool success = false;
	const auto begin = text.data();
	const auto end = begin + text.size();
	const auto ite = std::find(begin, end, separator);
	if (ite == end)
	{
		result = device_index(std::string(begin, ite), 0);
		success = true;
	}
	else if (ite != begin)
	{
		std::size_t id;
		if (std::from_chars(std::next(ite), end, id, 10).ec == std::errc())
		{
			result = device_index(
				std::string(begin, ite),
				id
			);
			success = true;
		}
	}

	return success;
}

} // namespace hardware
} // namespace xmipp4
