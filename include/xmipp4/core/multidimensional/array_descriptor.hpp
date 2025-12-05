// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "strided_layout.hpp"
#include "../numerical_type.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace multidimensional
{

class array_descriptor
{
public:
	array_descriptor() noexcept;
	array_descriptor(strided_layout layout, numerical_type data_type) noexcept;
	array_descriptor(const array_descriptor &other) = default;
	array_descriptor(array_descriptor &&other) = default;
	~array_descriptor() = default;

	array_descriptor& operator=(const array_descriptor &other) = default;
	array_descriptor& operator=(array_descriptor &&other) = default;

	const strided_layout& get_layout() const noexcept;

	numerical_type get_data_type() const noexcept;

private:
	strided_layout m_layout;
	numerical_type m_data_type;
};

// TODO operators

} // namespace multidimensional
} // namespace xmipp4

