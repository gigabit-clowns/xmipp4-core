// SPDX-License-Identifier: GPL-3.0-only

#include "array_implementation.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
array_implementation::array_implementation(
	std::shared_ptr<hardware::buffer> storage,
	array_descriptor descriptor
) noexcept
	: m_storage(std::move(storage))
	, m_descriptor(std::move(descriptor))
{
}

inline
const array_descriptor& array_implementation::get_descriptor() const noexcept
{
	return m_descriptor;
}

inline
hardware::buffer* array_implementation::get_storage() const noexcept
{
	return m_storage.get();
}

inline
const std::shared_ptr<hardware::buffer>& 
array_implementation::share_storage() const noexcept
{
	return m_storage;
}

} // namespace multidimensional
} // namespace xmipp4
