// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/ndarray/array_view.hpp>

#include <xmipp4/core/ndarray/array.hpp>

#include "array_implementation.hpp"

namespace xmipp4 
{

array_view::array_view() = default;
array_view::array_view(const array_view& other) = default;
array_view::array_view(array_view&& other) noexcept = default;
array_view::~array_view() = default;
array_view& array_view::operator=(array_view&& other) noexcept = default;
array_view& array_view::operator=(const array_view& other) = default;

array_view::array_view(
	const array &other
) noexcept
	: array_view(other.share())
{
}

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

const buffer* array_view::get_storage() const noexcept
{
	return
		m_implementation ? 
		m_implementation->get_storage() : 
		nullptr;
}

std::shared_ptr<const buffer> 
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

} // namespace xmipp4
