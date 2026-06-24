// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_signature.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

#include <boost/functional/hash.hpp>

namespace xmipp4 
{
namespace multidimensional
{

namespace 
{

const hardware::memory_resource* get_memory_resource(const hardware::buffer *storage)
{
	if (!storage)
	{
		return nullptr;
	}

	return &storage->get_memory_resource();
}

} // anonymous namespace

array_signature::array_signature() noexcept
	: m_descriptor()
	, m_memory_region(nullptr)
{
}

array_signature::array_signature(
	const array_descriptor &descriptor,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(descriptor)
	, m_memory_region(resource)
{
}

array_signature::array_signature(
	array_descriptor &&descriptor,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(std::move(descriptor))
	, m_memory_region(resource)
{
}

array_signature::array_signature(
	const strided_layout& layout,
	numerical_type data_type,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(layout, data_type)
	, m_memory_region(resource)
{
}

array_signature::array_signature(
	strided_layout&& layout,
	numerical_type data_type,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(std::move(layout), data_type)
	, m_memory_region(resource)
{
}

array_signature::array_signature(
	const array_signature &other
) = default;
array_signature::array_signature(
	array_signature &&other
) noexcept = default;
array_signature::~array_signature() = default;

array_signature& 
array_signature::operator=(const array_signature &other) = default;
array_signature& 
array_signature::operator=(array_signature &&other) noexcept = default;

bool array_signature::operator==(const array_signature &other) const noexcept
{
	return 
		get_descriptor() == other.get_descriptor() &&
		get_memory_resource() == other.get_memory_resource();
}

bool array_signature::operator!=(const array_signature &other) const noexcept
{
	return !(*this == other);
}

std::size_t array_signature::array_signature::hash() const noexcept
{
	auto seed = m_descriptor.hash();
	boost::hash_combine(seed, boost::hash_value(m_memory_region));
	return seed;
}

const strided_layout& array_signature::get_layout() const noexcept
{
	return m_descriptor.get_layout();
}

numerical_type array_signature::get_data_type() const noexcept
{
	return m_descriptor.get_data_type();
}

const array_descriptor& array_signature::get_descriptor() const noexcept
{
	return m_descriptor;
}

void array_signature::set_descriptor(
	const array_descriptor &descriptor
) noexcept
{
	m_descriptor = descriptor;
}

const hardware::memory_resource* 
array_signature::get_memory_resource() const noexcept
{
	return m_memory_region;
}

void array_signature::set_memory_resource(
	const hardware::memory_resource* resource
) noexcept
{
	m_memory_region = resource;
}

array_signature 
array_signature::from_array(const array &a) noexcept
{
	return array_signature(
		a.get_descriptor(),
		multidimensional::get_memory_resource(a.get_storage())
	);
}

array_signature 
array_signature::from_array(const array_view &a) noexcept
{
	return array_signature(
		a.get_descriptor(),
		multidimensional::get_memory_resource(a.get_storage())
	);
}

} // namespace multidimensional
} // namespace xmipp4

