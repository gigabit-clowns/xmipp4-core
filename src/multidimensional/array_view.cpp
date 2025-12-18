// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_view.hpp>

#include "array_implementation.hpp"

namespace xmipp4 
{
namespace multidimensional
{

array_view::array_view() = default;
array_view::array_view(array_view&& other) noexcept = default;
array_view::~array_view() = default;
array_view& array_view::operator=(array_view&& other) noexcept = default;

array_view::array_view(
	std::shared_ptr<const array_implementation> implementation
) noexcept
	: m_implementation(std::move(implementation))
{
}

const array_descriptor& array_view::get_descriptor() const noexcept
{
	static array_descriptor empty_descriptor;
	return
		m_implementation ? 
		m_implementation->get_descriptor() : 
		empty_descriptor;
}

const hardware::buffer* array_view::get_storage() const noexcept
{
	return
		m_implementation ? 
		m_implementation->get_storage() : 
		nullptr;
}

std::shared_ptr<const hardware::buffer> 
array_view::share_storage() const noexcept
{
	return
		m_implementation ? 
		m_implementation->share_storage() : 
		nullptr;
}

array_view array_view::share() const noexcept
{
	return array_view(m_implementation);
}

} // namespace multidimensional
} // namespace xmipp4
