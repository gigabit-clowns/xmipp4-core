// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_specification.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

namespace xmipp4 
{
namespace multidimensional
{

namespace 
{

hardware::memory_resource* get_memory_resource(const hardware::buffer *storage)
{
	if (!storage)
	{
		return nullptr;
	}

	return &storage->get_memory_resource();
}

} // anonymous namespace

array_specification::array_specification() noexcept
	: m_descriptor()
	, m_memory_region(nullptr)
{
}

array_specification::array_specification(
	const array_descriptor &descriptor,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(descriptor)
	, m_memory_region(resource)
{
}

array_specification::array_specification(
	const strided_layout& layout,
	numerical_type data_type,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(layout, data_type)
	, m_memory_region(resource)
{
}

array_specification::array_specification(
	strided_layout&& layout,
	numerical_type data_type,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(std::move(layout), data_type)
	, m_memory_region(resource)
{
}

array_specification::array_specification(
	const array_specification &other
) = default;
array_specification::array_specification(
	array_specification &&other
) noexcept = default;
array_specification::~array_specification() = default;

array_specification& 
array_specification::operator=(const array_specification &other) = default;
array_specification& 
array_specification::operator=(array_specification &&other) noexcept = default;

const strided_layout& array_specification::get_layout() const noexcept
{
	return m_descriptor.get_layout();
}

numerical_type array_specification::get_data_type() const noexcept
{
	return m_descriptor.get_data_type();
}

const array_descriptor& array_specification::get_descriptor() const noexcept
{
	return m_descriptor;
}

void array_specification::set_descriptor(
	const array_descriptor &descriptor
) noexcept
{
	m_descriptor = descriptor;
}

const hardware::memory_resource* 
array_specification::get_memory_resource() const noexcept
{
	return m_memory_region;
}

void array_specification::set_memory_resource(
	const hardware::memory_resource* resource
) noexcept
{
	m_memory_region = resource;
}

array_specification 
array_specification::from_array(const array &a) noexcept
{
	return array_specification(
		a.get_descriptor(),
		multidimensional::get_memory_resource(a.get_storage())
	);
}

array_specification 
array_specification::from_array(const array_view &a) noexcept
{
	return array_specification(
		a.get_descriptor(),
		multidimensional::get_memory_resource(a.get_storage())
	);
}

/*
bool operator==(
	const array_specification &lhs, 
	const array_specification &rhs
) noexcept;

bool operator!=(
	const array_specification &lhs, 
	const array_specification &rhs
) noexcept;
*/

} // namespace multidimensional
} // namespace xmipp4

