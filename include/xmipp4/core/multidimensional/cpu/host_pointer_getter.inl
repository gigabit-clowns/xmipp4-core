// SPDX-License-Identifier: GPL-3.0-only

#include "host_pointer_getter.hpp"

#include <stdexcept>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
inline
T* host_pointer_getter::operator()(hardware::buffer &buffer) const
{
	auto *result = static_cast<T*>(buffer.get_host_ptr());

	if (!result)
	{
		throw std::invalid_argument("Provided buffer is not host accessible");
	}

	return result;
}

template <typename T>
inline
const T* host_pointer_getter::operator()(const hardware::buffer &buffer) const
{
	const auto *result = static_cast<T*>(buffer.get_host_ptr());

	if (!result)
	{
		throw std::invalid_argument("Provided buffer is not host accessible");
	}

	return result;
}

} // namespace multidimensional
} // namespace xmipp4
