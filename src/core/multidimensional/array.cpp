// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array.hpp>

#include "array_implementation.hpp"

namespace xmipp4 
{
namespace multidimensional
{

array::array() = default;
array::array(array&& other) noexcept = default;
array::~array() = default;
array& array::operator=(array&& other) noexcept = default;

array::array(
	std::shared_ptr<hardware::buffer> storage,
	array_descriptor descriptor
)
	: array(
		std::make_shared<array_implementation>(
			std::move(storage), 
			std::move(descriptor)
		)
	)
{
}

array::array(
	std::shared_ptr<const array_implementation> implementation
) noexcept
	: m_implementation(std::move(implementation))
{
}

const array_descriptor& array::get_descriptor() const noexcept
{
	static array_descriptor empty_descriptor;
	return
		m_implementation ? 
		m_implementation->get_descriptor() : 
		empty_descriptor;
}

hardware::buffer* array::get_storage() noexcept
{
	return
		m_implementation ? 
		m_implementation->get_storage() : 
		nullptr;
}

const hardware::buffer* array::get_storage() const noexcept
{
	return
		m_implementation ? 
		m_implementation->get_storage() : 
		nullptr;
}

std::shared_ptr<hardware::buffer> array::share_storage() noexcept
{
	return
		m_implementation ? 
		m_implementation->share_storage() : 
		nullptr;
}

std::shared_ptr<const hardware::buffer> array::share_storage() const noexcept
{
	return
		m_implementation ? 
		m_implementation->share_storage() : 
		nullptr;
}

array array::share() noexcept
{
	return array(m_implementation);
}

array_view array::share() const noexcept
{
	return array_view(m_implementation);
}

} // namespace multidimensional
} // namespace xmipp4
