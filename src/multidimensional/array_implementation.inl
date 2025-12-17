// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_implementation.hpp"

namespace xmipp4 
{
namespace multidimensional
{

array_implementation::array_implementation(
	std::shared_ptr<hardware::buffer> storage,
	array_descriptor descriptor
) noexcept
	: m_storage(std::move(storage))
	, m_descriptor(std::move(descriptor))
{
}

const array_descriptor& array_implementation::get_descriptor() const noexcept
{
	return m_descriptor;
}

hardware::buffer* array_implementation::get_storage() const noexcept
{
	return m_storage.get();
}

const std::shared_ptr<hardware::buffer>& 
array_implementation::share_storage() const noexcept
{
	return m_storage;
}

} // namespace multidimensional
} // namespace xmipp4
