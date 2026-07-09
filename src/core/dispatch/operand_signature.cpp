// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/dispatch/operand_signature.hpp>

#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/array_view.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

#include <boost/functional/hash.hpp>

namespace xmipp4
{
namespace dispatch
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

operand_signature::operand_signature() noexcept
	: m_descriptor()
	, m_memory_region(nullptr)
{
}

operand_signature::operand_signature(
	const ndarray::array_descriptor &descriptor,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(descriptor)
	, m_memory_region(resource)
{
}

operand_signature::operand_signature(
	ndarray::array_descriptor &&descriptor,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(std::move(descriptor))
	, m_memory_region(resource)
{
}

operand_signature::operand_signature(
	const layout::strided_layout& layout,
	numerical_type data_type,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(layout, data_type)
	, m_memory_region(resource)
{
}

operand_signature::operand_signature(
	layout::strided_layout&& layout,
	numerical_type data_type,
	const hardware::memory_resource* resource
) noexcept
	: m_descriptor(std::move(layout), data_type)
	, m_memory_region(resource)
{
}

operand_signature::operand_signature(
	const operand_signature &other
) = default;
operand_signature::operand_signature(
	operand_signature &&other
) noexcept = default;
operand_signature::~operand_signature() = default;

operand_signature&
operand_signature::operator=(const operand_signature &other) = default;
operand_signature&
operand_signature::operator=(operand_signature &&other) noexcept = default;

bool operand_signature::operator==(
	const operand_signature &other
) const noexcept
{
	return
		get_descriptor() == other.get_descriptor() &&
		get_memory_resource() == other.get_memory_resource();
}

bool operand_signature::operator!=(
	const operand_signature &other
) const noexcept
{
	return !(*this == other);
}

std::size_t operand_signature::hash() const noexcept
{
	auto seed = m_descriptor.hash();
	boost::hash_combine(seed, boost::hash_value(m_memory_region));
	return seed;
}

const layout::strided_layout& operand_signature::get_layout() const noexcept
{
	return m_descriptor.get_layout();
}

numerical_type operand_signature::get_data_type() const noexcept
{
	return m_descriptor.get_data_type();
}

const ndarray::array_descriptor&
operand_signature::get_descriptor() const noexcept
{
	return m_descriptor;
}

void operand_signature::set_descriptor(
	const ndarray::array_descriptor &descriptor
) noexcept
{
	m_descriptor = descriptor;
}

const hardware::memory_resource*
operand_signature::get_memory_resource() const noexcept
{
	return m_memory_region;
}

void operand_signature::set_memory_resource(
	const hardware::memory_resource* resource
) noexcept
{
	m_memory_region = resource;
}

operand_signature
operand_signature::from_array(const ndarray::array &a) noexcept
{
	return operand_signature(
		a.get_descriptor(),
		dispatch::get_memory_resource(a.get_storage())
	);
}

operand_signature
operand_signature::from_array(const ndarray::array_view &a) noexcept
{
	return operand_signature(
		a.get_descriptor(),
		dispatch::get_memory_resource(a.get_storage())
	);
}

} // namespace dispatch
} // namespace xmipp4
