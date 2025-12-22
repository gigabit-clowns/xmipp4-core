// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_descriptor.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array_descriptor::array_descriptor() noexcept
	: m_data_type(numerical_type::unknown)
{
}

array_descriptor::array_descriptor(
	strided_layout layout, 
	numerical_type data_type
) noexcept
	: m_layout(std::move(layout))
	, m_data_type(data_type)
{
}

array_descriptor::array_descriptor(const array_descriptor &other) = default;
array_descriptor::array_descriptor(array_descriptor &&other) noexcept = default;
array_descriptor::~array_descriptor() = default;

array_descriptor&
array_descriptor::operator=(const array_descriptor &other) = default;
array_descriptor&
array_descriptor::operator=(array_descriptor &&other) noexcept = default;

const strided_layout& array_descriptor::get_layout() const noexcept
{
	return m_layout;
}

numerical_type array_descriptor::get_data_type() const noexcept
{
	return m_data_type;
}

bool operator==(
	const array_descriptor &lhs, 
	const array_descriptor &rhs
) noexcept
{
	return 
		lhs.get_data_type() == rhs.get_data_type() &&
		lhs.get_layout() == rhs.get_layout();
}

bool operator!=(
	const array_descriptor &lhs, 
	const array_descriptor &rhs
) noexcept
{
	return !(lhs == rhs);
}

bool is_initialized(const array_descriptor &descriptor) noexcept
{
	return 
		descriptor.get_data_type() != numerical_type::unknown ||
		descriptor.get_layout() == strided_layout();

}

std::size_t compute_storage_requirement(const array_descriptor &descriptor)
{
	const auto &layout = descriptor.get_layout();
	const auto data_type = descriptor.get_data_type();
	return layout.compute_storage_requirement() * get_size(data_type);
}

} // namespace multidimensional
} // namespace xmipp4
